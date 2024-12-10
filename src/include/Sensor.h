//
// Created by root on 11/14/24.
//

#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"


class Sensor : public Object
{
public:
    Sensor(Game* game, float radius, int team);
    void before() override;
    void step() override;
    void after() override;

    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //SENSOR_H
