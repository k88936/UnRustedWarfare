//
// Created by root on 11/5/24.
//

#ifndef UNIT_H
#define UNIT_H
#include "Drawable.h"
#include "MetaUnit.h"
#include "Turret.h"


class Unit :public Attachable{
public:
    explicit Unit(MetaUnit * meta,QVector3D position,float rotation);

    void updatePosition(QMatrix4x4 transform, float rotation_base) override;


    QVector3D velocity;
    QVector3D acceleration;
    MetaUnit *meta = nullptr;

    bool isAttached=false;

    void attack(QVector3D target);
    void draw() override;
    std::vector<attachSlot<Turret>> turrets;

};



#endif //UNIT_H
