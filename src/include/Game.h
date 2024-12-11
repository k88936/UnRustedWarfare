//
// Created by root on 11/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <QBasicTimer>
#include <QTime>
#include <set>
#include <unordered_map>
#include <vector>
#include "AudioManager.h"
#include "battlefieldwidget.h"
#include "GridsManager.h"
#include "MapConfig.h"
#include "../game/config/GameConfig.h"
#include "structures/sound_event.h"


class Flock;
class Effect;
class Projectile;
class Unit;

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

    class TimerDoer : public QObject
    {
        QOBJECT_H

        Game* game;

    public:
        explicit TimerDoer(Game* game) : game(game)
        {
        }

        QTime start = QTime::currentTime();

        void timerEvent(QTimerEvent* event) override
        {
            game->step();
            // game->delta_time = start.msecsTo(QTime::currentTime()) * GameConfig::game_speed / 1000.0+0.00001;
            game->delta_time=0.01;
            start = QTime::currentTime();
        };
    };

    explicit Game(BattlefieldWidget* battlefield_widget, const std::string& world);
    ~Game();

    std::unordered_map<std::string, std::vector<Drawable*>> var_solid_image_draw_config_map;
    std::vector<QVector3D> line_draw_config;
    std::unordered_map<std::string, std::vector<Drawable*>> const_image_draw_config_map;
    float delta_time = 0;
    GridsManager grids_manager;
    MapConfig map_config;
    AudioManager audio_manager;
    std::vector<Unit*> units;
    QBasicTimer timer;
    std::unordered_map<std::string, std::vector<Drawable*>> ui_image_draw_config_map;
    QTime start_time = QTime::currentTime();
    std::unordered_map<std::string, std::vector<Drawable*>> var_transparent_image_draw_config_map;
    std::vector<Projectile*> projectiles;
    std::vector<Effect*> effects;
    std::set<Flock*> flocks;
    BattlefieldWidget* battle_field_widget;
    void add_projectile(Projectile* projectile);
    void add_effect(Effect* effect);
    void start(const std::string& world);
    TimerDoer* timer_doer = new TimerDoer(this);
    void clean();
    void pause();
    void run();
    void stop();
    void step();
};


#endif //GAME_H
