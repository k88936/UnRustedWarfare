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

Game::Game(): audio_manager(this)
{
}

// float Game::deltaTime = 0;
//
// std::vector<QVector3D> Game::line_draw_config;
// std::unordered_map<std::string, std::vector<Drawable*>> Game::var_solid_image_draw_config_map;
// std::unordered_map<std::string, std::vector<Drawable*>> Game::var_transparent_image_draw_config_map;
// std::unordered_map<std::string, std::vector<Drawable*>> Game::const_image_draw_config_map;
// std::unordered_map<std::string, std::vector<Drawable*>> Game::ui_image_draw_config_map;
// std::unordered_map<std::string, std::vector<SoundEvent>> Game::sound_event_config_map;
// GridsManager Game::grids_manager;
// AudioManager Game::audio_manager;
// std::vector<Unit*> Game::units;
// std::set<Flock*> Game::flocks;
// QBasicTimer Game::timer;
// std::vector<Projectile*> Game::projectiles;
// std::vector<Effect*> Game::effects;
// BattlefieldWidget* Game::battle_field_widget;
// QTime Game::start_time = QTime::currentTime();
// Game::TimerDoer* Game::timer_doer = new Game::TimerDoer();
//
void Game::addProjectile(Projectile* projectile)
{
    Game::projectiles.push_back(projectile);
}

void Game::addEffect(Effect* effect)
{
    Game::effects.push_back(effect);
}


void Game::start_on(const std::string& map_path, BattlefieldWidget* widget)
{
    Game::battle_field_widget = widget;
    map_config.loadMap(map_path);
    grids_manager.init(this);
    for (const auto& tile : map_config.tiles)
    {
        tile->draw(this);
    }
    resume();
    for (int i = 25; i < 35; ++i)
    {
        for (int j = 25; j < 35; ++j)
        {
            Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 0, QVector3D(i, j, 0), i + j));
        }
    }
    Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("m2a3"), 0, QVector3D(30, 43, 0), -20));
    Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 1, QVector3D(37, 32, 0), -20));

    for (int i = 57; i < 60; ++i)
    {
        for (int j = 40; j < 43; ++j)
        {
            Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 1, QVector3D(i, j, 0), i + j));
        }
    }
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

void Game::pause()
{
    timer.stop();
}

void Game::resume()
{
    // if (timer.isActive())
    timer.start(0, timer_doer);
}

void Game::stop()
{
    pause();
    for (auto unit : units)delete unit;
    units.clear();
    for (auto projectile : projectiles)delete projectile;
    projectiles.clear();
    for (auto effect : effects)delete effect;
    effects.clear();
    for (auto flock : flocks)delete flock;
    flocks.clear();
    Game::var_solid_image_draw_config_map.clear();
    Game::var_transparent_image_draw_config_map.clear();
    Game::const_image_draw_config_map.clear();
    Game::ui_image_draw_config_map.clear();
    line_draw_config.clear();
    Game::sound_event_config_map.clear();
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
