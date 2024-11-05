// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwidget.h"

#include <QMouseEvent>

#include <cmath>

// MainWidget::MainWidget(QWidget *parent) {
// }

MainWidget::~MainWidget()
= default;


//! [1]
void MainWidget::timerEvent(QTimerEvent *) {
    update();
}

//! [1]

void MainWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);
    initTextures();
    engine = new RenderEngine(texture);
    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]

//! [3]

//! [4]
void MainWidget::initTextures() {
    // Load cube.png image
    QOpenGLTexture *tex = new QOpenGLTexture(QImage("../DJ.png").mirrored());

    // Set nearest filtering mode for texture minification
    tex->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    tex->setMagnificationFilter(QOpenGLTexture::Linear);
    texture.push_back(tex);
    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    // texture->setWrapMode(QOpenGLTexture::);
}

//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h) {
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

void MainWidget::paintGL() {
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
    //! [2]


    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    engine->setView(projection * matrix);

    // Draw cube geometry
    engine->render();
}
