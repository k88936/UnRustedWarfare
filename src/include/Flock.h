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
    void move(const QVector3D& target);


    std::set<Unit*> boids;
    bool mark_as_delete = false;
    QVector3D center;
    QVector3D speed;
    FlowField* flow_field = nullptr;
    QVector3D pos_target;
    float arrived_range;

    void gather(const QVector3D& target);
};


#endif //FLOCK_H
