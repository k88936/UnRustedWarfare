//
// Created by root on 11/5/24.
//

#ifndef METATURRET_H
#define METATURRET_H
#include "Game.h"
#include "MetaAttachable.h"
#include "MetaObject.h"
#include "MetaDrawable.h"
#include "utils.h"

class MetaTurret :public MetaAttachable ,public  MetaObject,public MetaDrawable
{
    // QVector4D barrelTransform;
public:
    MetaTurret();
    QVector3D barrel_position=QVector3D(0,0,GameConfig::LayerConfig::TURRET_OFFSET);
    float turn_speed=-1;
    float turn_acc;
    QVector4D  shoot_light;
    std::string shoot_sound=utils::EMPTY_STR;
    float shoot_sound_volume=1;

    float recoil_offset=0;
    float recoil_out_time=0;
    float recoil_return_time=0;

    float range=-1;
    float range_min=-1;
    float delay;
    float idle_dir=0;
    float turn_speed_acceleration;
    std::string attached_to;
    std::vector<MetaTurret*> attached_turrets=std::vector<MetaTurret*>();

    bool can_shoot=true;
    float limiting_min_range;
    std::string projectile;
    bool slave=false;
    std::vector<std::string> shoot_flame;
    bool invisible=false;
    void init() override;
};



#endif //METATURRET_H
