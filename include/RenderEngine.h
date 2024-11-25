// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <qimage.h>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <Structures.h>


class RenderEngine : protected QOpenGLFunctions {
public:
    RenderEngine();

    virtual ~RenderEngine();

    void initShaders();

    void transform(const QMatrix4x4 &matrix);

    void setColor(const QVector4D &color);

    void render();

    void setView(const QMatrix4x4 &matrix4_x4);

    void resisterTexture(const std::string &, const MetaImage &metaImage);
    void bindTexture(const std::string& id, int index);
    void bindTexture(const std::string&);
    void bindShaderProgram();
private:
    std::unordered_map<std::string,std::pair<QOpenGLTexture*,QOpenGLVertexArrayObject*> > textures;
    QOpenGLShaderProgram program;
    QMatrix4x4 view;
};

#endif // GEOMETRYENGINE_H
