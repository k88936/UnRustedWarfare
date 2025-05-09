//
// Created by root on 11/10/24.
//

#ifndef TURRET_H
#define TURRET_H
#include "Attachable.h"
#include "Drawable.h"
#include "MetaTurret.h"
#include "Sensor.h"
#include "utils.h"


class Turret : public Attachable, public Drawable, public Sensor
{
public:
    Turret(Game* game, MetaTurret* meta, int team);
    ~Turret();
    MetaTurret* meta;
    float rotationSpeed = 0;
    float coolDown = 0;
    Object* current_target = nullptr;
    Object* preferred_target = nullptr;
    bool has_target = false;
    bool is_aimed = false;
Drawable*     shadow=new Drawable();

    utils::animater recoil_animater;
    bool is_driving;
    void updateSlots(QMatrix4x4 transform) override;
    std::vector<Turret*> turrets_attached;
    float aim(QVector3D target);
    bool shoot();
    bool attack(const QVector3D& target);
    void draw(Game* game) override;


    void before() override;
    void step() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D position_diff) override;
};


#endif //TURRET_H
