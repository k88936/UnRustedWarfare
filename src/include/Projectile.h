//
// Created by root on 11/12/24.
//

#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Drawable.h"
#include "MetaProjectiles.h"
#include "Unit.h"


class Projectile : public Drawable, public Object
{
public:
    float has_lived = 0;
    MetaProjectiles* meta;

    Projectile(Game* game, MetaProjectiles* meta, int team, QVector3D position, float rotation,
               const QVector3D& linear_velocity_base);
    void draw(Game* game) override;
    void before() override;
    void after() override;
    bool on_overlay(Object* obj, QVector3D positionDiff) override;

    void hit_effect(const Unit* unit) const;
};


#endif //PROJECTILE_H
