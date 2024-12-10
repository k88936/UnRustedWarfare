// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>

#include "structures/meta_image.h"


class RenderEngine : protected QOpenGLFunctions
{
public:
    RenderEngine();

    virtual ~RenderEngine();

    void init_shaders();

    void transform(const QMatrix4x4& matrix);

    void setColor(const QVector4D& color);

    void render();

    void set_view(const QMatrix4x4& matrix4_x4);

    void resister_texture(const std::string&, const MetaImage& metaImage);
    void bind_texture(const std::string&);
    void bind_texture_shader();
    void bind_simple_shader();

private:
    std::unordered_map<std::string, std::pair<QOpenGLTexture*, QOpenGLVertexArrayObject*>> textures_;
    QOpenGLShaderProgram texture_shader_;
    QOpenGLShaderProgram simple_shader_;
    QMatrix4x4 view_;
};

#endif // GEOMETRYENGINE_H
