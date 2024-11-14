//
// Created by root on 11/10/24.
//

#ifndef TURRET_H
#define TURRET_H
#include "Attachable.h"
#include "Drawable.h"
#include "MetaTurret.h"


class Turret :public Attachable,public Drawable,public Object{
public:
    Turret(MetaTurret* meta);
    MetaTurret *meta;
    float rotationSpeed;
    void updateSlots(QMatrix4x4 transform, float rotation_base) override;
    std::vector<Turret*> turrets_attached;
    float aim(QVector3D target);
    bool shoot();
    void draw() override;
};



#endif //TURRET_H
