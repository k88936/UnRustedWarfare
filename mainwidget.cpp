// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwidget.h"

#include <QMouseEvent>

#include <cmath>

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete engine;
    doneCurrent();
}


//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
        update();
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1);

    glClearColor(0, 1, 0, 1);

    engine = new RenderEngine();
    initTextures();


    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

//! [3]

//! [3]

//! [4]
void MainWidget::initTextures()
{
    engine->resisterTexture("DJ",QImage("../DJ.png"));

}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    qreal  zoom=0.01;
    const qreal zNear = 3.0, zFar = 7.0;

    // Reset projection
    projection.setToIdentity();
    projection.ortho(-w*zoom,w*zoom,-h*zoom,h*zoom,zNear,zFar);

    // Set perspective projection
    // projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{


//! [2]
    engine->bindTexture("DJ");

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    engine->setView(projection * matrix);

    // Draw cube geometry
    engine->render();
}
