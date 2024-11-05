// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "RenderEngine.h"

#include <QVector2D>
#include <QVector3D>
#include <utility>

void RenderEngine::initShaders() {
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../vshader.glsl")) {
        qDebug() << "Error in loading vertex shader";
        exit(1);
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../fshader.glsl")) {
        qDebug() << "Error in loading fragment shader";
        exit(1);
    }

    // Link shader pipeline
    if (!program.link()) {
        qDebug() << "Error in linking shader program";
        exit(1);
    }


    // Bind shader pipeline for use
    if (!program.bind()) {
        qDebug() << "Error in binding shader program";
        exit(1);
    }
    // Calculate model view transformation

    // Set modelview-projection matrix
    program.setUniformValue("texture", 0);
    //! [6]
}

struct VertexData {
    QVector3D position;
    QVector2D texCoord;
};

QOpenGLBuffer arrayBuf;
QOpenGLBuffer vao(QOpenGLBuffer::VertexBuffer);
//! [0.5]
RenderEngine::RenderEngine(std::vector<QOpenGLTexture *> textures) {
    initializeOpenGLFunctions();
    qDebug()<<"starting render engine";
    this->textures=std::move(textures);
    arrayBuf.create();
    vao.create();
    initShaders();
    initVOA();
}

RenderEngine::~RenderEngine()=default;

//! [0]

void RenderEngine::initVOA() {
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    float fac = 2.0;
    float ofst = 0.5;
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f * fac - ofst, -1.0f * fac, 1.0f * fac), QVector2D(0, 0)}, // v0
        {QVector3D(1.0f * fac - ofst, -1.0f * fac, 1.0f * fac), QVector2D(1, 0)}, // v1
        {QVector3D(1.0f * fac - ofst, 1.0f * fac, 1.0f * fac), QVector2D(1, 1)}, // v3
        {QVector3D(-1.0f * fac - ofst, 1.0f * fac, 1.0f * fac), QVector2D(0, 1)}, // v2
    };
    vao.bind();
    arrayBuf.bind();


    arrayBuf.allocate(vertices, 4 * sizeof(VertexData));
    // indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program.attributeLocation("a_position");
    program.enableAttributeArray(vertexLocation);
    program.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program.attributeLocation("a_texcoord");
    program.enableAttributeArray(texcoordLocation);
    program.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    arrayBuf.release();
    vao.release();

}

//! [2]
void RenderEngine::render() {
    textures[0]->bind();
    program.setUniformValue("mvp_matrix", view);

    vao.bind();
    QMatrix4x4 matrix;
    matrix.translate(0, 10, 0);
    matrix.rotate(45, 0, 0, 1);
    program.setUniformValue("transform", matrix);
    glDrawArrays(GL_QUADS, 0, 4);
    vao.release();
    textures[0]->release();
}

void RenderEngine::setView(QMatrix4x4 view) {
    this->view = view;
}


//! [2]
