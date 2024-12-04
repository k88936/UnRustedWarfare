//
// Created by root on 11/10/24.
//

#ifndef TURRET_H
#define TURRET_H
#include "Attachable.h"
#include "Drawable.h"
#include "MetaTurret.h"
#include "Sensor.h"


class Turret :public Attachable,public Drawable,public Sensor{
public:
    Turret(MetaTurret* meta,int team);
    ~Turret() override;
    MetaTurret *meta;
    float rotationSpeed=0;
    float coolDown=0;
    Object* current_target=nullptr;
    Object* preferred_target =nullptr;
    bool has_target=false;
    void updateSlots(QMatrix4x4 transform) override;
    std::vector<Turret*> turrets_attached;
    float aim(QVector3D target);
    bool shoot();
    bool attack(const QVector3D &target);
    void draw() override;


    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;

};



#endif //TURRET_H
