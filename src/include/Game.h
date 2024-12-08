//
// Created by root on 11/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <QBasicTimer>
#include <unordered_map>
#include <vector>

#include "AudioManager.h"
#include "battlefieldwidget.h"
#include "Drawable.h"
#include "Effect.h"
#include "FlowField.h"
#include "GridsManager.h"
#include "structures/sound_event.h"


class Unit;
class Projectile;
class Flock;

class Game
{
public:
    class LayerConfig
    {
    public:
        constexpr static float TILE_GROUND = -1;
        constexpr static float TILE_ITEM = -0.9;
        constexpr static float OBJECT_GROUND = 0;
        constexpr static float TURRET_OFFSET = 0.1;
        constexpr static float BOTTOM_EFFECT_OFFSET = -0.05;
        constexpr static float SHELL = 1;
        constexpr static float OBJECT_AIR = 2;
        constexpr static float UPPER_EFFECT_OFFSET = 0.05;
    };

    static std::unordered_map<std::string, std::vector<Drawable*>> var_solid_image_draw_config_map;
    static std::vector<QVector3D> line_draw_config;
    static std::unordered_map<std::string, std::vector<Drawable*>> const_image_draw_config_map;
    static float FPS;
    static float deltaTime;
    static GridsManager grids_manager;
    static AudioManager audio_manager;
    static std::vector<Unit*> units;
    static QBasicTimer timer;
    static std::unordered_map<std::string, std::vector<Drawable*>> ui_image_draw_config_map;
    static QTime start_time;
    static std::unordered_map<std::string, std::vector<Drawable*>> var_transparent_image_draw_config_map;
    static std::unordered_map<std::string, std::vector<struct SoundEvent>> sound_event_config_map;
    static std::vector<Projectile*> projectiles;
    static std::vector<Effect*> effects;
    static std::set<Flock*> flocks;
    static BattlefieldWidget* battle_field_widget;
    static float volume;
    static void addProjectile(Projectile* projectile);
    static void addEffect(Effect* effect);
    static void init();
    static void clean();
    static void step();
};


#endif //GAME_H
