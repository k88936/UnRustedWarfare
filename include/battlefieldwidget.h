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

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void batch_draw(std::unordered_map<std::string, std::vector<Drawable*>>& batches) const;
    void paintGL() override;
    void initShaders();
    void update_textures();
    float x_center = 50;
    float y_center = 50;
    float zoom = 0.05;

private:
    QOpenGLShaderProgram program;
    RenderEngine* engine = nullptr;
    QOpenGLTexture* texture = nullptr;
    QMatrix4x4 projection;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
