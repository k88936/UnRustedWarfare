// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>


class RenderEngine : protected QOpenGLFunctions {
public:

    RenderEngine(std::vector<QOpenGLTexture *> textures);
    void initShaders();

    virtual ~RenderEngine();

    void render();

    void setView(QMatrix4x4);

    void getShaderProgram();

    QOpenGLShaderProgram program;

private:
    QMatrix4x4 view;
    std::vector<QOpenGLTexture *> textures;

    void initVOA();
};

#endif // GEOMETRYENGINE_H
