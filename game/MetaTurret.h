//
// Created by root on 11/5/24.
//

#ifndef METATURRET_H
#define METATURRET_H
#include "MetaAttachable.h"
#include "MetaObject.h"


class MetaTurret :public MetaAttachable ,public  MetaObject{
    // QVector4D barrelTransform;
public:
    MetaTurret();
    float x;
    float y;
    float barrelX;
    float barrelY;
    std::string image;
    float turnSpeed=-1;
    float turnAcc;
    std::string shootSound;
    float shootSoundVolume;

    float recoilOffset;
    float recoilOutTime;
    float recoilReturnTime;

    float range;
    float rangeMin;
    float delay;
    bool invisible=false;
    float turnSpeedAcceleration;
    std::string attachedTo;
    std::vector<MetaTurret*> attached_turrets=std::vector<MetaTurret*>();

    bool canShoot;
    float limitingMinRange;
    std::string projectile;
    bool slave;
    std::string shoot_effect;
};



#endif //METATURRET_H
