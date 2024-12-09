// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <Drawable.h>
#include <QOpenGLWidget>
#include "RenderEngine.h"


class BattlefieldWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    using QOpenGLWidget::QOpenGLWidget;
    explicit BattlefieldWidget(QWidget* parent = nullptr);
    QVector3D screen_to_world(const QPointF& screen_pos) const;
    QVector3D screen_relative_to_world_relative(const QPointF& screen_relative) const;
    ~BattlefieldWidget() override;
    QVector3D camera_pos = QVector3D(50, 50, 5);
    float camera_zoom = 0.05;
    void update_camera();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void batch_draw(std::unordered_map<std::string, std::vector<Drawable*>>& batches) const;
    void paintGL() override;
    void update_textures();

private:
    QOpenGLShaderProgram program;
    RenderEngine* engine = nullptr;
    QMatrix4x4 projection;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
