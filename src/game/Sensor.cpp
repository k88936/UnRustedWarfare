//
// Created by root on 11/14/24.
//

#include "Sensor.h"
Sensor::Sensor(const float radius): Object(radius,0,0) {

}

void Sensor::step()
{
    Object::step();
}

bool Sensor::on_overlay(Object *obj, const QVector3D position_diff) {
    return false;
}
