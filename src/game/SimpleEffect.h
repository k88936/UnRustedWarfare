//
// Created by root on 12/5/24.
//

#ifndef SIMPLEEFFECT_H
#define SIMPLEEFFECT_H
#include "Effect.h"


class SimpleEffect : public Effect
{
public:
    float max_life;
    std::string image;


    SimpleEffect(std::string image, float life, QVector3D position, float rotation, float scale,
                 QVector3D linear_velocity,
                 float angular_velocity);
    void draw() override;
    void before() override;
    void after() override;
};


#endif //SIMPLEEFFECT_H