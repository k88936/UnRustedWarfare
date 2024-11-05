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
{
    initializeOpenGLFunctions();
    initShaders();

    // Generate 2 VBOs

    // Initializes cube geometrrey and transfers it to VBOs
}

RenderEngine::~RenderEngine()
{
}
//! [0]
void RenderEngine::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../vshader.glsl"))
        exit(1);

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../fshader.glsl"))
        exit(1);

    // Link shader pipeline
    if (!program.link())
        exit(1);

    // Bind shader pipeline for use
    if (!program.bind())
        exit(1);
}


//! [2]
void RenderEngine::render()
{
    program.bind();

//! [6]
    // Calculate model view transformation

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", view );
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    QMatrix4x4 matrix;
    matrix.translate(0, 10, 0);
    matrix.rotate(45, 0, 0, 1);
    program.setUniformValue("transform", matrix);
    // Draw cube geometry using indices from VBO 1
    // glDrawElements(GL_TRIANGLE_STRIP, 5, GL_UNSIGNED_SHORT, nullptr);
    glDrawArrays(GL_QUADS, 0, 4);
}

void RenderEngine::setView(const QMatrix4x4 &matrix4_x4) {
    view = matrix4_x4;
}

void RenderEngine::resisterTexture(std::string id, QImage image ) {  // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    QOpenGLBuffer *arrayBuf=new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);;
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    arrayBuf->create();
    const float factor=0.01;
    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-factor*image.width(), -factor*image.height(),  0), QVector2D(0, 0)},  // v0
        {QVector3D( factor*image.width(), -factor*image.height(),  0), QVector2D(1, 0)}, // v1
        {QVector3D( factor*image.width(),  factor*image.height(),  0), QVector2D(1, 1)}, // v3
        {QVector3D(-factor*image.width(),  factor*image.height(),  0), QVector2D(0, 1)},  // v2
    };
    vao->bind();
    arrayBuf->bind();
    arrayBuf->allocate(vertices, 4 * sizeof(VertexData));
    // Tell OpenGL which VBOs to use
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
    arrayBuf->release(   );
    vao->release();

   QOpenGLTexture* texture = new QOpenGLTexture(image.mirrored());


    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Linear);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    textures[id] = std::make_pair(texture, vao);
    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    // texture->setWrapMode(QOpenGLTexture::);
}

void RenderEngine::bindTexture(std::string id) {

    textures[id].first->bind();
    textures[id].second->bind();
}

//! [2]
