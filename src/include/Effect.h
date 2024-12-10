//
// Created by root on 11/18/24.
//

#ifndef EFFECT_H
#define EFFECT_H
#include "Drawable.h"
#include "MetaEffect.h"
#include "Object.h"


class Effect : public Object, public Drawable
{
public:
    float has_life = 0;
    float animate_timer = 0;

    MetaEffect* meta = nullptr;

    Effect(Game* game);
    Effect(Game* game, MetaEffect* meta, QVector3D position, float rotation, const QVector3D& linear_velocity_base);
    void draw(Game* game) override;
    void before() override;
    void step() override;
    void after() override;
};


#endif //EFFECT_H
