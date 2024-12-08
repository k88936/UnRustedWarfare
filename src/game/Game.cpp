//
// Created by root on 11/10/24.
//

#include "Game.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>

#include <QObject>

#include "AudioManager.h"
#include "battle_widget.h"
#include "UnitConfigs.h"

#include "Unit.h"
#include "Effect.h"
#include "Flock.h"
#include "FlowField.h"
#include "MapConfig.h"
#include "PathFind.h"
#include "Projectile.h"
#include "Tile.h"
#include "utils.h"

float Game::deltaTime = 1.0 / 40;
float Game::FPS = 1.0 / deltaTime;

std::vector<QVector3D> Game::line_draw_config;
std::unordered_map<std::string, std::vector<Drawable*>> Game::var_solid_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable*>> Game::var_transparent_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable*>> Game::const_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable*>> Game::ui_image_draw_config_map;

std::unordered_map<std::string, std::vector<SoundEvent>> Game::sound_event_config_map;
GridsManager Game::grids_manager;
AudioManager Game::audio_manager;
std::vector<Unit*> Game::units;
std::set<Flock*> Game::flocks;
QBasicTimer Game::timer;
std::vector<Projectile*> Game::projectiles;
std::vector<Effect*> Game::effects;
BattlefieldWidget* Game::battle_field_widget;
QTime Game::start_time = QTime::currentTime();

void Game::addProjectile(Projectile* projectile)
{
    Game::projectiles.push_back(projectile);
}

void Game::addEffect(Effect* effect)
{
    Game::effects.push_back(effect);
}


auto ef = QSoundEffect();

void Game::init()
{
    // ef.setSource(QUrl(":/b.ogg"));
    // ef.setLoopCount(10);
    // ef.setMuted(false);
    // ef.setVolume(1.0);
    // ef.play();
    //

    UnitConfigs::init();

    MapConfig::init();
    MapConfig::loadMap("../maps/2.tmx");
    //
    // MapConfig::world_width=100;
    // MapConfig::world_height=100;
    grids_manager.init();
    audio_manager.init();
    // battleFieldWidget->show();
    // welcome = new welcome_widget();
    // battleFieldWidget = welcome->get_battleFieldWidget();
    // welcome->show();
    auto battle = new battle_widget();
    battle_field_widget = battle->get_battleFieldWidget();
    battle->show();
    for (const auto& tile : MapConfig::tiles)
    {
        tile->draw();
    }
    // welcome->setWidget( battleFieldWidget);
    class TimerDoer : public QObject
    {
        QOBJECT_H

    public:
        void timerEvent(QTimerEvent* event) override
        {
            Game::step();
        };
    };
    timer.start(static_cast<int>(deltaTime * 1000), new TimerDoer());

    for (int i = 25; i < 35; ++i)
    {
        for (int j = 25; j < 35; ++j)
        {
            Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 0, QVector3D(i, j, 0), i + j));
        }
    }

    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), 0, QVector3D(30, 43, 0), -20));
    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 1, QVector3D(37, 32, 0), -20));

    // Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 1, QVector3D(30, 20, 0), 50));
    // // Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 1, QVector3D(41, 41, 0), 50));
    // // Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 1, QVector3D(40, 40, 0), 50));
    //
    for (int i = 57; i < 60; ++i)
    {
        for (int j = 40; j < 43; ++j)
        {
            Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), 1, QVector3D(i, j, 0), i + j));
        }
    }
    // Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), QVector3D(133, 120, 0), -5));


    // flow_field_for_test = new FlowField(80, 50, movementType::LAND);
    // // Game::line_draw_config.clear();
    // //
    // for (int i = 0; i < MapConfig::world_width; ++i)
    // {
    //     for (int j = 0; j < MapConfig::world_height; ++j)
    //     {
    //         Game::line_draw_config.emplace_back(i, j, 0);
    //         Game::line_draw_config.emplace_back(flow_field_for_test->get_vector(i, j) * 0.4 + QVector3D(i, j, 0));
    //     }
    // }


    // for (int i = 0; i < 50; ++i)
    // {
    //     for (int j = 0; j < 50; ++j)
    //     {
    // Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), QVector3D(i*2+1, 2*j+1, 0), 150));
    //     }
    // }
}

void Game::clean()
{
    for (auto it = Game::projectiles.begin(); it != Game::projectiles.end();)
    {
        if ((*it)->marked_for_delete)
        {
            delete *it;
            it = Game::projectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = Game::effects.begin(); it != Game::effects.end();)
    {
        if ((*it)->marked_for_delete)
        {
            delete *it;
            it = Game::effects.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto it = Game::units.begin(); it != Game::units.end();)
    {
        if ((*it)->marked_for_delete && (*it)->reference_count == 0)
        {
            (*it)->on_death();
            delete *it;
            it = Game::units.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto flock : flocks)
    {
        if (flock->boids.empty())
        {
            flock->mark_as_delete = true;
        }
    }
    for (auto it = Game::flocks.begin(); it != Game::flocks.end();)
    {
        if ((*it)->mark_as_delete)
        {
            delete *it;
            it = Game::flocks.erase(it);
        }
        else
        {
            ++it;
        }
    }
    Game::grids_manager.clear_grids();
    Game::var_solid_image_draw_config_map.clear();
    Game::var_transparent_image_draw_config_map.clear();
}

void Game::step()
{
    clean();
    for (const auto u : Game::units)
    {
        u->before();
    }
    for (const auto p : Game::projectiles)
    {
        p->before();
    }
    for (const auto& e : Game::effects)
    {
        e->before();
    }

    for (const auto u : Game::units)
    {
        u->step();
    }
    for (const auto u : Game::units)
    {
        u->after();
    }
    for (const auto p : Game::projectiles)
    {
        p->after();
    }
    for (const auto& e : Game::effects)
    {
        e->after();
    }
    audio_manager.play(battle_field_widget->camera_pos);
    battle_field_widget->update();
}
