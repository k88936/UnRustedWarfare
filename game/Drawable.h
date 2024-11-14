//
// Created by root on 11/5/24.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <QMatrix4x4>

#include "Object.h"

// struct DrawableData {
    //
    // };

class Drawable {
public:

    Drawable();

    std::string textureId;
    QMatrix4x4 render_transform;
    QVector4D color=QVector4D(1,1,1,1);
    float scale=1;
    virtual void draw();
};


#endif //DRAWABLE_H
