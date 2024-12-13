//
// Created by root on 11/5/24.
//

#ifndef DRAWABLE_H
#define DRAWABLE_H
#include <QMatrix4x4>


class Game;

class Drawable
{
public:
    int frame_id = 0;
    QMatrix4x4 render_transform;
    QVector4D color = QVector4D(1, 1, 1, 1);
    // QVector4D color = QVector4D(0.3, 0.3, 0.3, 1);
    bool with_shadow = false;
    float scale = 1;
    virtual void draw(Game* game);
};


#endif //DRAWABLE_H
