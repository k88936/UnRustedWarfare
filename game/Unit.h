//
// Created by root on 11/5/24.
//

#ifndef UNIT_H
#define UNIT_H
#include "Body.h"
#include "Drawable.h"
#include "MetaUnit.h"
#include "Turret.h"


class Unit :public Attachable,public Drawable,public Body{
public:
    explicit Unit(MetaUnit * meta,QVector3D position,float rotation);
    void updateSlots(QMatrix4x4 transform, float rotation_base) override;
    MetaUnit *meta = nullptr;
    bool isAttached=false;
    void attack(QVector3D target);
    void draw() override;
    std::vector<Turret*> turrets;
    std::vector<Object*> watchers;

};



#endif //UNIT_H
