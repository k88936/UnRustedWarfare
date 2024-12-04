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
    float angular_target=0;
    QVector3D speed_target;
    int nearby_count=1;
    float nearby_even_rotation=0;
    QVector3D nearby_even_speed;
    QVector3D nearby_even_center;
    Flock* flock = nullptr;
    QVector3D target_offset;

    BoidSensor(const float radius, Unit* boid): Sensor(radius, boid->team)
    {
        this->boid = boid;
    }

    ~BoidSensor() override;

    bool arrived_flock_target = false;
    bool arrived_flock_target_offset = false;
    Unit* boid;
    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //BOIDSENSOR_H
