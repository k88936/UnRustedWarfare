//
// Created by root on 11/5/24.
//

#ifndef UNIT_H
#define UNIT_H
#include "Drawable.h"
#include "MetaUnit.h"
#include "Turret.h"



class Unit : public Attachable, public Drawable, public Object
{
public:
    explicit Unit(MetaUnit* meta, QVector3D position, float rotation);
    void updateSlots(QMatrix4x4 transform) override;
    MetaUnit* meta = nullptr;
    bool isAttached = false;
    void attack(const QVector3D& target);
    void draw() override;
    std::vector<Turret*> turrets;
    std::vector<Object*> watchers;

    void before() override;
    void after() override;
};


#endif //UNIT_H
