//
// Created by root on 11/14/24.
//

#include "Sensor.h"
Sensor::Sensor(const float radius,int team): Object(radius,0,0) {

    this->team=team;
}

void Sensor::before()
{
    Object::before();
}

void Sensor::step()
{
    Object::step();
}

void Sensor::after()
{
}

bool Sensor::on_overlay(Object *obj, const QVector3D position_diff) {
    return false;
}
