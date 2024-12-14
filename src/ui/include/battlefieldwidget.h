// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QOpenGLWidget>

#include "RenderEngine.h"


class Game;
class Drawable;

class BattlefieldWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    using QOpenGLWidget::QOpenGLWidget;
    QVector3D screen_to_world(const QPointF& screen_pos) const;
    QVector3D screen_relative_to_world_relative(const QPointF& screen_relative) const;
    virtual void dialog(const std::string& string);
    virtual void info(const std::string& string);
    BattlefieldWidget(QWidget* parent);
    ~BattlefieldWidget() override;
    QVector3D camera_pos = QVector3D(5, 30, 5);
    float camera_zoom = 0.02;
    void update_camera();
    virtual void render();
    Game* game;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void batch_draw(std::unordered_map<std::string, std::vector<Drawable*>>& batches);
    void paintGL() override;
    void update_textures();

private:
    QOpenGLShaderProgram program;
    RenderEngine* engine = nullptr;
    QMatrix4x4 projection;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
