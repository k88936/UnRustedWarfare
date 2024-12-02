//
// Created by root on 11/6/24.
//

#ifndef UNIT_CONFIGS_H
#define UNIT_CONFIGS_H
#include <qimage.h>

#include "MetaEffect.h"
#include "MetaProjectiles.h"
#include "MetaUnit.h"
#include <unordered_map>
#include <string>

#include "Effect.h"
#include "Projectile.h"
#include "structures/meta_image.h"


class UnitConfigs {
public:
    static std::unordered_map<std::string, MetaUnit *> meta_units;
    static std::unordered_map<std::string, MetaProjectiles *> meta_projectiles;
    static std::unordered_map<std::string, MetaEffect *> meta_effects;
    static std::unordered_map<std::string,MetaImage> images;
    static void parse_effects(const std::string& content, std::vector<std::string>& ans);
    static void load_ini(const QString &path);
    static void init();
};


#endif //CONFIGS_H
