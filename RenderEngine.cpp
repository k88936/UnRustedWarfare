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
RenderEngine::RenderEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Initializes cube geometry and transfers it to VBOs
    initCubeGeometry();
}

RenderEngine::~RenderEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
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

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    // GLushort indices[] = {
    //      0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
        //  4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
        //  8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        // 12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
        // 16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
        // 20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
    // };

//! [1]
    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 4 * sizeof(VertexData));

    // Transfer index data to VBO 1
    // indexBuf.bind();
    // indexBuf.allocate(indices, 32 * sizeof(GLushort));
//! [1]
}

//! [2]
void RenderEngine::render(QOpenGLShaderProgram *program)
{
    program->setUniformValue("texture", 0);
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    // indexBuf.bind();

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


    QMatrix4x4 matrix;
    matrix.translate(0, 10, 0);
    matrix.rotate(45, 0, 0, 1);
    program->setUniformValue("transform", matrix);
    // Draw cube geometry using indices from VBO 1
    // glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, nullptr);
    glDrawArrays(GL_QUADS, 0, 4);
}
//! [2]
