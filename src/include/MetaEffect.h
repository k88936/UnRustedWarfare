//
// Created by root on 11/5/24.
//

#ifndef METAEFFECT_H
#define METAEFFECT_H
#include <string>
#include <vector>

#include "MetaDrawable.h"
#include "MetaObject.h"

enum Priority {
    high = 0,
    medium = 1,
    low = 2
};

class MetaEffect :public MetaObject ,public  MetaDrawable{
public:
    bool attachedToUnit;
    int animateFrameStart=0;
    int animateFrameEnd=0;
    float animateFrameDelay=1;

    float life; //secends
    float scaleFrom=1;
    float scaleTo=1;
    float xSpeedRelativeRandom=0;
    float ySpeedRelativeRandom=0;
    float xSpeedRelative=0;
    float ySpeedRelative=0;
    float xSpeedAbsolute=0;
    float ySpeedAbsolute=0;
    float spawnChance=1;
    float fadeInTime=0;
    bool fadeOut=false;
    float alpha=1;
    float hSpeed;
    bool drawUnderUnits=false;
    Priority priority;
    bool atmospheric;
    float delayedStartTimer;
    std::string alsoPlaySound;
    float alsoPlaySoundVolume;
};


#endif //METAEFFECT_H
