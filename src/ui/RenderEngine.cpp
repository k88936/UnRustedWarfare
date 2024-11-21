// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "RenderEngine.h"

#include <QVector2D>

#include <QVector3D>
#include <QOpenGLBuffer>

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


void RenderEngine::transform(const QMatrix4x4& matrix)
{
    program.setUniformValue("transform", matrix);
}

void RenderEngine::setColor(const QVector4D& color)
{
    program.setUniformValue("color", color);
}

//! [2]
void RenderEngine::render()
{
    ;
    glDrawArrays(GL_QUADS, 0, 4);
}

void RenderEngine::setView(const QMatrix4x4& view)
{
    program.setUniformValue("mvp_matrix", view);
}

void RenderEngine::resisterTexture(const std::string& id, const MetaImage& metaImage)
{
    for (int i = 0; i < metaImage.frames; ++i)
    {
        QRect rect = QRect(i * metaImage.image.width() / metaImage.frames, 0,
                           metaImage.image.width() / metaImage.frames, metaImage.image.height());
        QOpenGLBuffer* arrayBuf = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        auto* vao = new QOpenGLVertexArrayObject();
        vao->create();
        arrayBuf->create();
        VertexData vertices[4];
        if (metaImage.rawSize)
        {
            vertices[0] = {
                QVector3D(-static_cast<float>(rect.height()) /80, -static_cast<float>(rect.width()) / 80, 0),
                QVector2D(0, 0)
            }; // v0
            vertices[1] = {
                QVector3D(static_cast<float>(rect.height()) /80, -static_cast<float>(rect.width()) / 80, 0),
                QVector2D(0, 1)
            }; // v1
            vertices[2] = {
                QVector3D(static_cast<float>(rect.height()) /80, static_cast<float>(rect.width()) / 80, 0),
                QVector2D(1, 1)
            }; // v3
            vertices[3] = {
                QVector3D(-static_cast<float>(rect.height()) /80, static_cast<float>(rect.width()) / 80, 0),
                QVector2D(1, 0)
            }; // v2
        }
        else
        {
            // Vertex data for face 0
            vertices[0] = {
                QVector3D(
                    -static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5,
                    -0.5, 0),
                QVector2D(0, 0)
            }; // v0
            vertices[1] = {
                QVector3D(
                    static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5,
                    -0.5, 0),
                QVector2D(0, 1)
            }; // v1
            vertices[2] = {
                QVector3D(
                    static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5,
                    0.5, 0),
                QVector2D(1, 1)
            }; // v3
            vertices[3] = {
                QVector3D(
                    -static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5,
                    0.5, 0),
                QVector2D(1, 0)
            }; // v2
        }
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
        arrayBuf->release();
        vao->release();

        QOpenGLTexture* texture = new QOpenGLTexture(metaImage.image.copy(rect).mirrored());
        // Set nearest filtering mode for texture minification
        texture->setMinificationFilter(QOpenGLTexture::Linear);
        // Set bilinear filtering mode for texture magnification
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        textures[(id + "_"+std::to_string(i))] = std::make_pair(texture, vao);
    }
}


void RenderEngine::bindTexture(const std::string& id)
{
    textures.at(id).first->bind();
    textures.at(id).second->bind();
}

void RenderEngine::bindShaderProgram()
{
    this->program.bind();
}

//! [2]
