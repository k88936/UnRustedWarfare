//
// Created by root on 11/6/24.
//

#ifndef CONFIGS_H
#define CONFIGS_H
#include "MetaEffect.h"
#include "MetaTurret.h"
#include "MetaProjectiles.h"
#include "MetaUnit.h"
#include <unordered_map>


class Configs {
public:
    static std::unordered_map<std::string, MetaUnit *> meta_units;
    static std::unordered_map<std::string, MetaTurret *> meta_turrets;
    static std::unordered_map<std::string, MetaProjectiles *> meta_projectiles;
    static std::unordered_map<std::string, MetaEffect *> meta_effects;

    static void init();
};


#endif //CONFIGS_H
