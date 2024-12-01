// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "RenderEngine.h"

#include <QVector2D>

#include <QOpenGLBuffer>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

std::unordered_map<std::string, std::pair<QOpenGLTexture*, QOpenGLVertexArrayObject*>> RenderEngine::textures;
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
    if (!texture_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/t_vshader.glsl"))
        throw std::runtime_error("vertex shader compile error");
    // Compile fragment shader
    if (!texture_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/t_fshader.glsl"))
        throw std::runtime_error("fragment shader compile error");
    // Link shader pipeline
    if (!texture_shader.link())
        throw std::runtime_error("shader link error");



    if (!simple_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/s_vshader.glsl"))
        throw std::runtime_error("vertex shader compile error");
    if (!simple_shader.link())
        throw std::runtime_error("shader link error");
    // // Bind shader pipeline for use
    // if (!texture_shader.bind())
    //     throw std::runtime_error("shader bind error");
}


void RenderEngine::transform(const QMatrix4x4& matrix)
{
    texture_shader.setUniformValue("transform", matrix);
}

void RenderEngine::setColor(const QVector4D& color)
{
    texture_shader.setUniformValue("color", color);
}

//! [2]
void RenderEngine::render()
{
    glDrawArrays(GL_QUADS, 0, 4);
}

void RenderEngine::setView(const QMatrix4x4& view)
{
    texture_shader.setUniformValue("mvp_matrix", view);
    simple_shader.setUniformValue("mvp_matrix", view);
}

void RenderEngine::resisterTexture(const std::string& id, const MetaImage& metaImage)
{
    if (textures.contains(id))
    {
        return;
    }
    if (id=="NONE")
    {
        return;
    }
    if (metaImage.image.isNull())
    {
        throw std::runtime_error("image is null");
    }
    QOpenGLVertexArrayObject* vao;
    QOpenGLTexture* texture;
    for (int i = 0; i < metaImage.frames; ++i)
    {
        QRect rect = QRect(i * metaImage.image.width() / metaImage.frames, 0,
                           metaImage.image.width() / metaImage.frames, metaImage.image.height());
        auto* arrayBuf = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        vao = new QOpenGLVertexArrayObject();
        vao->create();
        arrayBuf->create();
        VertexData vertices[4];
        QVector2D texCoords[4];
        if (metaImage.rawRot)
        {
            texCoords[0] = QVector2D(0, 0);
            texCoords[1] = QVector2D(1, 0);
            texCoords[2] = QVector2D(1, 1);
            texCoords[3] = QVector2D(0, 1);
        }
        else
        {
            texCoords[0] = QVector2D(0, 0);
            texCoords[1] = QVector2D(0, 1);
            texCoords[2] = QVector2D(1, 1);
            texCoords[3] = QVector2D(1, 0);
        }
        if (metaImage.is_raw_size)
        {
            vertices[0] = {
                QVector3D(-static_cast<float>(rect.height()) * metaImage.scale / 40,
                          -static_cast<float>(rect.width()) * metaImage.scale / 40, 0),
                texCoords[0]
            }; // v0
            vertices[1] = {
                QVector3D(static_cast<float>(rect.height()) * metaImage.scale / 40,
                          -static_cast<float>(rect.width()) * metaImage.scale / 40, 0),
                texCoords[1]
            }; // v1
            vertices[2] = {
                QVector3D(static_cast<float>(rect.height()) * metaImage.scale / 40,
                          static_cast<float>(rect.width()) * metaImage.scale / 40, 0),
                texCoords[2]
            }; // v3
            vertices[3] = {
                QVector3D(-static_cast<float>(rect.height()) * metaImage.scale / 40,
                          static_cast<float>(rect.width()) * metaImage.scale / 40, 0),
                texCoords[3]
            }; // v2
        }
        else
        {
            // Vertex data for face 0
            vertices[0] = {
                QVector3D(
                    -static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5 * metaImage.scale,
                    -0.5 * metaImage.scale, 0),
                texCoords[0]
            }; // v0
            vertices[1] = {
                QVector3D(
                    static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5 * metaImage.scale,
                    -0.5 * metaImage.scale, 0),
                texCoords[1]
            }; // v1
            vertices[2] = {
                QVector3D(
                    static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5 * metaImage.scale,
                    0.5 * metaImage.scale, 0),
                texCoords[2]
            }; // v3
            vertices[3] = {
                QVector3D(
                    -static_cast<float>(rect.height()) / static_cast<float>(rect.width()) * 0.5 * metaImage.scale,
                    0.5 * metaImage.scale, 0),
                texCoords[3]
            }; // v2
        }
        vao->bind();
        arrayBuf->bind();
        arrayBuf->allocate(vertices, 4 * sizeof(VertexData));
        // Tell OpenGL which VBOs to use
        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = texture_shader.attributeLocation("a_position");
        texture_shader.enableAttributeArray(vertexLocation);
        texture_shader.setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = texture_shader.attributeLocation("a_texcoord");
        texture_shader.enableAttributeArray(texcoordLocation);
        texture_shader.setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));
        arrayBuf->release();
        vao->release();

        texture = new QOpenGLTexture(metaImage.image.copy(rect).mirrored());
        // Set nearest filtering mode for texture minification
        texture->setMinificationFilter(QOpenGLTexture::Linear);
        // Set bilinear filtering mode for texture magnification
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        textures[(id + "_" + std::to_string(i))] = std::make_pair(texture, vao);
    }
    if (metaImage.frames == 1)
    {
        textures[id] = std::make_pair(texture, vao);
    }
}


void RenderEngine::bindTexture(const std::string& id)
{
    textures.at(id).first->bind();
    textures.at(id).second->bind();
}

void RenderEngine::bind_texture_shader()
{
    this->texture_shader.bind();
}

void RenderEngine::bind_simple_shader()
{
    this->simple_shader.bind();
}

//! [2]
