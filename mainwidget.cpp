// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "mainwidget.h"

#include <QMouseEvent>

#include <cmath>

#include "game/Configs.h"
#include "game/Game.h"
#include "game/SpaceEngine.h"
#include "game/Unit.h"

Unit* unit;
Unit* unit2;
Unit* unit3;

SpaceEngine* spaceEngine;
MainWidget::MainWidget(QWidget *parent) {
    unit= new Unit(Configs::meta_units["m2a3"], QVector3D(0, 0, 0), 0);
    unit2= new Unit(Configs::meta_units["m2a3"], QVector3D(0, -3, 0), 0);
    unit3= new Unit(Configs::meta_units["m2a3"], QVector3D(7, -3, 0), 30);

    spaceEngine = new SpaceEngine();
}

MainWidget::~MainWidget() {
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete engine;
    doneCurrent();
}


float ths=0;
//! [1]
void MainWidget::timerEvent(QTimerEvent *) {
    Game::image_draw_config_map.clear();
  ths+=0.3;
    if(ths>=180) {
        ths-=360;
    }else if(ths<-180) {
        ths+=360;
    }
    // qDebug()<<ths;
    unit->relative_rotation=ths;
    unit->position=QVector3D(5.0/180*ths,3,0);
    unit->turrets[0]->aim({0,-3,0});
    unit->updateSlots(QMatrix4x4(), 0);
    unit->draw();


    unit2->relative_rotation=41*ths;
    unit2->updateSlots(QMatrix4x4(),0);
    unit2->draw();

    unit3->turrets[0]->aim(unit->position);
    unit3->updateSlots(QMatrix4x4(),0);
    unit3->draw();

    spaceEngine->updateObject(unit);

    update();
}

//! [1]

void MainWidget::initializeGL() {
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
    initTextures();
    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}
//! [4]
void MainWidget::initTextures() {
    // engine->resisterTexture("DJ", QImage("../DJ.png"));
    for (const auto &[id, image] : Configs::images) {
        engine->resisterTexture(id,image);
    }
}

//! [4en]

//! [5]
void MainWidget::resizeGL(int w, int h) {
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

void MainWidget::paintGL() {
    glClearColor(0, 1, 0, 1);
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
