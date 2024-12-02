//
// Created by root on 12/2/24.
//

#ifndef BOIDSENSOR_H
#define BOIDSENSOR_H
#include "Sensor.h"
#include "Unit.h"


class BoidSensor : public Sensor
{
public:
    float angular_target;
    float speed_target;
    int nearby_count;
    float nearby_even_rotation;
    QVector3D nearby_even_speed;

    BoidSensor(const float radius, Unit* boid): Sensor(radius)
    {
        this->boid = boid;
    }

    Unit* boid;
    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //BOIDSENSOR_H
