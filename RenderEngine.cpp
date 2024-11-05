// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "RenderEngine.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0.5]
RenderEngine::RenderEngine(QOpenGLShaderProgram *program)
    : vao(QOpenGLBuffer::VertexBuffer)
{
    this->program=program;
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    vao.create();

    // Initializes cube geometrrey and transfers it to VBOs
    initCubeGeometry();
}

RenderEngine::~RenderEngine()
{
    arrayBuf.destroy();
    vao.destroy();
}
//! [0]

void RenderEngine::initCubeGeometry()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0, 0)},  // v0
        {QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(1, 0)}, // v1
        {QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(1, 1)}, // v3
        {QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0, 1)},  // v2
    };
    vao.bind();
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 4 * sizeof(VertexData));program->setUniformValue("texture", 0);
    // Tell OpenGL which VBOs to use
    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
    arrayBuf.release(   );
    vao.release();


}

//! [2]
void RenderEngine::render()
{

    vao.release();
    QMatrix4x4 matrix;
    matrix.translate(0, 10, 0);
    matrix.rotate(45, 0, 0, 1);
    program->setUniformValue("transform", matrix);
    // Draw cube geometry using indices from VBO 1
    // glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, nullptr);
    glDrawArrays(GL_QUADS, 0, 4);
}
//! [2]
