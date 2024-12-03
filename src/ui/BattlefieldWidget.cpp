// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "battlefieldwidget.h"

#include <QMouseEvent>

#include <cmath>
#include <QOpenGLBuffer>
#include <qpainter.h>

#include "UnitConfigs.h"
#include "Game.h"
#include "Unit.h"
#include "MapConfig.h"

BattlefieldWidget::BattlefieldWidget(QWidget* parent)
{
}


BattlefieldWidget::~BattlefieldWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete engine;
    doneCurrent();
}

float ths = 0;

void BattlefieldWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.3);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    engine = new RenderEngine();
    update_textures();
    // Use QBasicTimer because its faster than QTimer
}

//! [4]
void BattlefieldWidget::update_textures()
{
    //for ui
    engine->resisterTexture("_select", MetaImage(QImage(":ui/select_highlight.png"), 2, false, false));
    engine->resisterTexture("_arrow_highlight", MetaImage(QImage(":ui/arrow_highlight.png"), 2, false, false));
    engine->resisterTexture("_arrow_orange", MetaImage(QImage(":ui/arrow_orange.png"), 2, false, false));

    for (const auto& [id, image] : UnitConfigs::images)
    {
        if (id.empty())continue;
        engine->resisterTexture(id, image);
    }
    for (const auto& [id, meta_image] : MapConfig::tile_images)
    {
        if (id.empty())continue;
        engine->resisterTexture(id, meta_image);
    }
}

void BattlefieldWidget::update_camera()
{
    // qDebug()<<camera_pos;
    // qDebug()<<camera_zoom;
    constexpr qreal zNear = 3.0, zFar = 7.0;
    projection.setToIdentity();
    projection.ortho(-this->size().width() * camera_zoom / 2 + camera_pos.x(), this->size().width() * camera_zoom / 2 + camera_pos.x(),
                     -this->size().height() * camera_zoom / 2 + camera_pos.y(),
                     this->size().height() * camera_zoom / 2 + camera_pos.y(), zNear, zFar);
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -camera_pos.z());
    // matrix.rotate(0,0,0,1);
    projection *= matrix;
}

void BattlefieldWidget::resizeGL(int w, int h)
{
    update_camera();
}

void BattlefieldWidget::batch_draw(std::unordered_map<std::string, std::vector<Drawable*>>& batches) const
{
    for (const auto& [texture_id, drawables] : batches)
    {
        if (texture_id=="NONE")continue;
        engine->bindTexture(texture_id);
        // qDebug()<<texture_id;
        for (const auto& drawable : drawables)
        {
            engine->transform(drawable->render_transform);
            engine->setColor(drawable->color);
            engine->render();
        }
    }
}

//! [5]
void BattlefieldWidget::paintGL()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    engine->bind_simple_shader();
    engine->setView(projection);
    glColor3f(48.0 / 256, 246.0 / 256, 217.0 / 256);
    glBegin(GL_LINES);
    for (const auto& line : Game::line_draw_config)
    {
        glVertex3f(line.x(), line.y(), line.z());
    }
    glEnd();

    engine->bind_texture_shader();
    engine->setView(projection);
    batch_draw(Game::const_image_draw_config_map);
    batch_draw(Game::var_image_draw_config_map);
    batch_draw(Game::ui_image_draw_config_map);
}

QVector3D BattlefieldWidget::screen_to_world(const QPointF& screen_pos) const
{
    const QPointF relate = screen_pos - this->pos();
    float x = relate.x() * camera_zoom + camera_pos.x() - this->size().width() * camera_zoom / 2;
    float y = -relate.y() * camera_zoom + camera_pos.y() + this->size().height() * camera_zoom / 2;
    return QVector3D(x, y, 0);
}

QVector3D BattlefieldWidget::screen_relative_to_world_relative(const QPointF& screen_relative) const
{
    return QVector3D(screen_relative.x() * camera_zoom, -screen_relative.y() * camera_zoom, 0);
}
