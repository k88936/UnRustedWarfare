//
// Created by root on 12/2/24.
//

#ifndef BOIDSENSOR_H
#define BOIDSENSOR_H
#include "Sensor.h"
#include "Unit.h"
#include "../game/Controller.h"


class BoidSensor : public Sensor, public Controller
{
public:
    float angular_target = 0;
    QVector3D speed_target;
    int nearby_count = 1;
    float nearby_even_rotation = 0;
    QVector3D nearby_even_speed;
    QVector3D nearby_even_center;
    Flock* flock = nullptr;
    QVector3D target_offset;
    bool can_drive = true;

    BoidSensor(Game* game, const float radius, Unit* boid): Sensor(game, radius, boid->team), Controller(boid)
    {
    }
    BoidSensor(Unit* unit);

    ~BoidSensor();

    bool arrived_flock_target = false;
    bool arrived_flock_target_offset = false;
    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //BOIDSENSOR_H
