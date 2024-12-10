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
        TimerDoer(Game* game) : game(game)
        {
        }

        QTime start = QTime::currentTime();


        void timerEvent(QTimerEvent* event) override
        {
            game->step();
            game->deltaTime = start.msecsTo(QTime::currentTime()) / 1000.0;
            start = QTime::currentTime();
        };
    };

    Game();

    std::unordered_map<std::string, std::vector<Drawable*>> var_solid_image_draw_config_map;
    std::vector<QVector3D> line_draw_config;
    std::unordered_map<std::string, std::vector<Drawable*>> const_image_draw_config_map;
    float deltaTime = 0;
    GridsManager grids_manager;
    AudioManager audio_manager;
    std::vector<Unit*> units;
    QBasicTimer timer;
    std::unordered_map<std::string, std::vector<Drawable*>> ui_image_draw_config_map;
    QTime start_time = QTime::currentTime();
    std::unordered_map<std::string, std::vector<Drawable*>> var_transparent_image_draw_config_map;
    std::unordered_map<std::string, std::vector<struct SoundEvent>> sound_event_config_map;
    std::vector<Projectile*> projectiles;
    std::vector<Effect*> effects;
    std::set<Flock*> flocks;
    BattlefieldWidget* battle_field_widget;
    MapConfig map_config;
    void addProjectile(Projectile* projectile);
    void addEffect(Effect* effect);
    void init();
    TimerDoer* timer_doer = new TimerDoer(this);

    void start_on(const std::string& map_path, BattlefieldWidget* widget);
    void clean();
    void pause();
    void resume();
    void stop();
    void step();
};


#endif //GAME_H
