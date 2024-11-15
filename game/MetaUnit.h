//
// Created by root on 11/5/24.
//

#ifndef METAUNIT_H
#define METAUNIT_H
#include <string>
#include <vector>

#include "MetaAttachable.h"
#include "MetaTurret.h"

// land water sub
 enum movementType {
        LAND=0x100,
        AIR=0b111,
        HOVER=0b110,
        SUB=0b011,
    };

class MetaUnit :MetaAttachable {
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
    int total_frames = 1;
    std::string image;
    std::string image_wreak;
    float scale;

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
    float turretSize;
    float turretTurnSpeed;

    //movement
    movementType movement;
    float moveSpeed;
    float moveAcc;
    float moveDec;
    float maxTurnSpeed;
    float turnAcc;
    float targetHeight;
    bool joinGroup;

    std::vector<MetaTurret*> attached_turret=std::vector<MetaTurret*>();
    void attack();
};


#endif //METAUNIT_H
