// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QOpenGLWidget>
#include <QBasicTimer>
#include <QOpenGLTexture>

#include "RenderEngine.h"


class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    using QOpenGLWidget::QOpenGLWidget;
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    void initTextures();

private:
    QOpenGLShaderProgram program;
    RenderEngine *engine = nullptr;

    QOpenGLTexture *texture = nullptr;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
};

#endif // MAINWIDGET_H
