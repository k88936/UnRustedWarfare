//
// Created by root on 12/2/24.
//

#ifndef BOIDSENSOR_H
#define BOIDSENSOR_H
#include "Flock.h"
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
    QVector3D nearby_even_center;
    Flock* flock=nullptr;

    BoidSensor(const float radius, Unit* boid): Sensor(radius,boid->team)
    {
        this->boid = boid;
    }
    ~BoidSensor()override ;

    Unit* boid;
    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //BOIDSENSOR_H
