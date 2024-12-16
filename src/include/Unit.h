//
// Created by root on 11/5/24.
//

#ifndef UNIT_H
#define UNIT_H
#include "Drawable.h"
#include "MetaUnit.h"
#include "Turret.h"


class Arm;
class Controller;
class BoidSensor;

class Unit : public Attachable, public Drawable, public Object
{
public:
    Unit(Game* game, MetaUnit* meta, int team, QVector3D position, float rotation);
    ~Unit();
    void updateSlots(QMatrix4x4 transform) override;
    MetaUnit* meta = nullptr;
    Drawable* shadow = new Drawable();
    Drawable* selection = new Drawable();
    Sensor* sight = nullptr;
    float hp;
    bool isAttached = false;
    Controller* controller = nullptr;
    bool is_driving = false;
    void attack(const QVector3D& target);
    void draw(Game* game) override;
    void drive(const QVector3D& force, float torque);
    std::vector<Turret*> turrets;
    std::vector<Arm*> arms;
    std::vector<Object*> watchers;

    void before() override;
    void step() override;
    void on_death();
    void on_drive();
    void on_new_selection();
    void on_move_order();
    void after() override;

    void on_collision(const QVector3D& force, float torque, Object* other) override;
};


#endif //UNIT_H
