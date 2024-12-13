//
// Created by root on 11/6/24.
//

#ifndef UNIT_CONFIGS_H
#define UNIT_CONFIGS_H
#include "MetaEffect.h"
#include "MetaProjectiles.h"
#include "MetaUnit.h"
#include <unordered_map>
#include <string>
#include "Projectile.h"
#include "structures/meta_image.h"


class UnitConfigs {
public:
    static std::unordered_map<std::string, MetaUnit *> meta_units;
    static std::unordered_map<std::string, MetaProjectiles *> meta_projectiles;
    static std::unordered_map<std::string, MetaEffect *> meta_effects;
    static std::unordered_map<std::string,MetaImage> images;
    static std::unordered_map<std::string, QUrl> sounds;
    static void load_ini(const QString &path);
    static void scan_dir(QString path);
    static void init();
    static void init_units_map();
    static std::unordered_map<std::string, std::string> mapped_unit_name;
};


#endif //CONFIGS_H
