//
// Created by root on 12/3/24.
//

#ifndef FLOCK_H
#define FLOCK_H
#include <set>
#include "Unit.h"


class FlowField;

class Flock
{
public:
    Flock(Game*);
    void move(const QVector3D& target);


    std::set<Unit*> boids;
    bool mark_as_delete = false;
    QVector3D center;
    QVector3D speed;
    FlowField* flow_field = nullptr;
    QVector3D pos_target;
    float arrived_range;
    Game* game;

    void gather(const QVector3D& target);
};


#endif //FLOCK_H
