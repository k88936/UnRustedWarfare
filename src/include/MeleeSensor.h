//
// Created by root on 12/17/24.
//

#ifndef MELEESENSOR_H
#define MELEESENSOR_H
#include "Controller.h"
#include "Sensor.h"


class Unit;

class MeleeSensor :public Sensor,public Controller{
public:
    MeleeSensor(Game* game, const float radius, Unit* unit);

    void before() override;
    void step() override;
    void after() override;

    bool on_overlay(Object* obj, QVector3D position_diff) override;
    QVector3D target;
    bool has_target;
};




#endif //MELEESENSOR_H
