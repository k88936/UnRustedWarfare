//
// Created by root on 11/5/24.
//

#ifndef METATURRET_H
#define METATURRET_H
#include "MetaAttachable.h"
#include "MetaObject.h"


class MetaTurret: MetaAttachable {
    // QVector4D barrelTransform;
public:
    float barrelX;
    float barrelY;
    std::string image;
    float turnSpeed;
    float turnAcc;
    std::string shootSound;
    float shootSoundVolume;

    float recoilOffset;
    float recoilOutTime;
    float recoilReturnTime;

    float range;
    float rangeMin;
    float delay;
    bool invisible;
    float turnSpeedAcceleration;
    std::string attachedTo;
    bool canShoot;
    float limitingMinRange;
    std::string projectile;
    bool slave;
    std::string shoot_effect;


    void attackAt();

};



#endif //METATURRET_H
