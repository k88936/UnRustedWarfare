// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "battlefieldwidget.h"

#include <QMouseEvent>

#include <cmath>

#include "Configs.h"
#include "Game.h"
#include "Unit.h"

BattlefieldWidget::BattlefieldWidget(QWidget *parent) {
}


BattlefieldWidget::~BattlefieldWidget() {
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete engine;
    doneCurrent();
}


float ths = 0;
//! [1]
//! [1]

void BattlefieldWidget::initializeGL() {
    initializeOpenGLFunctions(); // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Enable back face culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    engine = new RenderEngine();
    initTextures();
    // Use QBasicTimer because its faster than QTimer
}

//! [4]
void BattlefieldWidget::initTextures() {
    // engine->resisterTexture("DJ", QImage("../DJ.png"));
    for (const auto &[id, image]: Configs::images) {
        engine->resisterTexture(id, image);
    }
}

//! [4en]

//! [5]
void BattlefieldWidget::resizeGL(int w, int h) {
    // Calculate aspect ratio

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    qreal zoom = 0.01;
    const qreal zNear = 3.0, zFar = 7.0;

    // Reset projection
    projection.setToIdentity();
    projection.ortho(-w * zoom, w * zoom, -h * zoom, h * zoom, zNear, zFar);
    // Set perspective projection
    // projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]
void BattlefieldWidget::paintGL() {
    glClearColor(0.3, 0.3, 0.3, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    engine->bindShaderProgram();
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    engine->setView(projection * matrix);
    for (const auto &[texture_id, drawables]: Game::image_draw_config_map) {
        engine->bindTexture(texture_id);
        // qDebug()<<texture_id;
        for (const auto &drawable: drawables) {
            engine->transform(drawable->render_transform);
            engine->setColor(drawable->color);
            engine->render();
        }
    }
}
