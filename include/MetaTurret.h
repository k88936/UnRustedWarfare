//
// Created by root on 11/5/24.
//

#ifndef METATURRET_H
#define METATURRET_H
#include "MetaAttachable.h"
#include "MetaObject.h"
#include "MetaDrawable.h"

class MetaTurret :public MetaAttachable ,public  MetaObject,public MetaDrawable
{
    // QVector4D barrelTransform;
public:
    MetaTurret();
    QVector3D barrelPosition=QVector3D(0,0,0.1);
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

    bool canShoot=true;
    float limitingMinRange;
    std::string projectile;
    bool slave=false;
    std::vector<std::string> shoot_flame;
};



#endif //METATURRET_H
