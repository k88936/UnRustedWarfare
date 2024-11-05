// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "RenderEngine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class GeometryEngine;

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    using QOpenGLWidget::QOpenGLWidget;

    ~MainWidget() override;

protected:
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void initTextures();

private:
    QBasicTimer timer;
    RenderEngine *engine=nullptr;
    std::vector<QOpenGLTexture *> texture;
    QMatrix4x4 projection;
};

#endif // MAINWIDGET_H
