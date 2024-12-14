//
// Created by root on 12/14/24.
//

#ifndef ARM_H
#define ARM_H
#include "Attachable.h"


class MetaArm;

class Arm : public Attachable, public Drawable,public Object
{
public:

    Arm(Game* game, MetaArm* meta);
    void before() override;
    void step() override;
    void after() override;

    MetaArm* meta;
    void draw(Game* game) override;
};


#endif //ARM_H
