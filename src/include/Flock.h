//
// Created by root on 12/3/24.
//

#ifndef FLOCK_H
#define FLOCK_H
#include <set>

#include "FlowField.h"
#include "Unit.h"


class Flock
{
public:
    Flock();


    std::set<Unit*> boids;
    bool mark_as_delete = false;
    QVector3D center;
    QVector3D speed;
    FlowField* flow_field = nullptr;
    Object* preferred_target=nullptr;

    void move(const QVector3D& target);
};


#endif //FLOCK_H
