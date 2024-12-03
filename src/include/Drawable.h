//
// Created by root on 11/5/24.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <QMatrix4x4>
#include "Object.h"

class Drawable
{
public:
    int frame_id = 0;
    QMatrix4x4 render_transform;
    QVector4D color = QVector4D(1, 1, 1, 1);
    float scale = 1;
    virtual void draw();
    static QVector3D generate_random_small_vector(float maxOffset);
};


#endif //DRAWABLE_H
