//
// Created by root on 11/5/24.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <QVector3D>


class Object {
public:
    // Object(QVector3D position, float rotation);

    Object();
    QVector4D position=QVector4D(0,0,0,1);
    float rotation=0;//north is zero
};



#endif //OBJECT_H
