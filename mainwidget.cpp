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
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


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
    engine->resisterTexture("side1",QImage("../side1.png"));

}
//! [4en]

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
    glClearColor(0, 1, 0, 1);
    engine->bindShaderProgram();

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    engine->setView(projection * matrix);

    engine->bindTexture("DJ");
    QVector4D color(1,1,1,1);
    engine->setColor(color);

    QMatrix4x4 transform;
    transform.setToIdentity();
    transform.translate(0, 5, 0);
    transform.rotate(45, 0, 0, 1);
    transform.scale(2);
    engine->transform(transform);

    engine->render();

    engine->bindTexture("side1");

    transform.setToIdentity();
    transform.translate(0.2, 5, -0.1);
    transform.rotate(-45, 0, 0, 1);
    transform.scale(0.8);
    engine->transform(transform);

    engine->render();

}
