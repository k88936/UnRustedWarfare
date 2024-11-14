//
// Created by root on 11/5/24.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <QVector3D>

#include "Grid.h"

class Object {
public:
    Object();
    QVector3D position=QVector3D(0,0,0);
    float radius;
    float rotation=0;//east is zero
    std::vector<Grid *> gridsAcross;
};
#endif //OBJECT_H