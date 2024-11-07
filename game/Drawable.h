//
// Created by root on 11/5/24.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "Object.h"

// struct DrawableData {
    //
    // };

class Drawable :Object{

    std::string textureId="blank";
    QVector4D color=QVector4D(1,1,1,1);
    float scale=1;
    void draw();
};


#endif //DRAWABLE_H
