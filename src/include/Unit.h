//
// Created by root on 11/5/24.
//

#ifndef UNIT_H
#define UNIT_H
#include "Drawable.h"
#include "MetaUnit.h"
#include "Turret.h"


class BoidSensor;

class Unit : public Attachable, public Drawable, public Object
{
public:
    explicit Unit(MetaUnit* meta, int team, QVector3D position, float rotation);
    ~Unit() override;
    void updateSlots(QMatrix4x4 transform) override;
    MetaUnit* meta = nullptr;
    bool isAttached = false;
    BoidSensor* boid_sensor;
    Object* prefered_target = nullptr;
    void attack(const QVector3D& target);
    void draw() override;
    std::vector<Turret*> turrets;
    std::vector<Object*> watchers;

    void before() override;
    void step() override;
    void after() override;

};


#endif //UNIT_H
