//
// Created by root on 11/5/24.
//

#ifndef METAEFFECT_H
#define METAEFFECT_H
#include <string>

#include "MetaObject.h"

enum priority {
    high = 0,
    medium = 1,
    low = 2
};

class MetaEffect : MetaObject {
public:
    int totalFrames = 1;
    bool attachedToUnit;
    std::string image;
    int animateFrameStart;
    int animateFrameEnd;
    int animateFrameDelay;

    int life; //secends
    float scaleFrom;
    float scaleTo;
    float alpha;
    float xSpeedRelativeRandom;
    float ySpeedRelativeRandom;
    float hSpeed;
    bool drawUnderUnit;
    priority priority;
    bool atmospheric;
    float delayedStartTimer;
    std::string alsoPlaySound;
    float alsoPlaySoundVolume;
};


#endif //METAEFFECT_H
