//
// Created by root on 11/5/24.
//

#ifndef METAUNIT_H
#define METAUNIT_H
#include <string>
#include <vector>

#include "MetaAttachable.h"
#include "MetaDrawable.h"
#include "MetaTurret.h"
#include "structures/movement_type.h"


class MetaUnit :public  MetaAttachable ,public  MetaDrawable{
public:
    //core
    std::string name; //name
    std::string displayText; //display name
    std::string displayDescription;
    int price;
    float maxHp;
    float mass;
    int techLevel;
    float radius; //contact radius
    float displayRadius;
    float fogOfWarSightRange;
    int transportSlotsNeeded = 1;
    float buildSpeed;
    float energyMax;
    float energyRegen;
    std::vector<std::string> tags=std::vector<std::string>();

    //
    int maxTransportingUnits;
    std::vector<std::string> transportUnitsRequireTag=std::vector<std::string>();
    float exit_x;
    float exit_y;

    //graphics

    std::string image_wreak="NONE";

    //sounds
    std::vector<std::string> soundsOnNewSelection=std::vector<std::string>();
    std::vector<std::string> soundsOnMoveOrder=std::vector<std::string>();
    std::vector<std::string> soundsOnDeath=std::vector<std::string>();
    std::vector<std::string> soundsOnHit=std::vector<std::string>();
    std::vector<std::string> soundsOnFire=std::vector<std::string>();
    std::vector<std::string> soundsOnMove=std::vector<std::string>();


    //
    std::vector<std::string> effectOnDeath=std::vector<std::string>();


    //attack
    bool canAttack;
    bool canAttackFlyingUnits;
    bool canAttackLandUnits;
    bool canAttackUnderWaterUnits;
    float shootDelay;
    float maxAttackRange;
    bool showRangeUIGuide;
    bool turretMultiTargeting;
    float turretSize=1;
    float turretTurnSpeed;

    //movement
    movementType movement;
    float moveSpeed=0;
    float moveAcc;
    float moveDec;
    float maxTurnSpeed;
    float turnAcc;
    float targetHeight=0;
    bool joinGroup;

    std::vector<MetaTurret*> attached_turret=std::vector<MetaTurret*>();
    float scaleTurret=1;

    void attack();
};


#endif //METAUNIT_H
