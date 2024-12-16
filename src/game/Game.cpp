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
#include "MapConfig.h"
#include "PathFind.h"
#include "Projectile.h"
#include "Tile.h"

Game::Game(BattlefieldWidget* battlefield_widget, const std::string& world): warfare_fog_manager(this),
                                                                             battle_field_widget(battlefield_widget)
{
    map_config.init(this, world);
    battlefield_widget->game = this;
    grids_manager.init(this);
    warfare_fog_manager.init();
    for (const auto& tile : map_config.tiles)
    {
        tile->draw(this);
    }

    // for (int i = 20; i < 22; ++i)
    // {
    //     for (int j = 20; j < 21; ++j)
    //     {
    //         Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 0, QVector3D(i, j, 0), i + j));
    //     }
    // }
    // units.push_back(new Unit(this, UnitConfigs::meta_units.at("m2a3"), 0, QVector3D(32, 43, 0), -20));
    // units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 1, QVector3D(37, 32, 0), -20));


    // int x=1;
    // for (auto meta_unit : UnitConfigs::meta_units)
    // {
    //     int xpos = x%100;
    //     int ypos = (x/100)*4;
    //     units.push_back(new Unit(this, meta_unit.second, 0, QVector3D(xpos, ypos, 0), 0));
    //     units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 1, QVector3D(xpos+1, ypos+1, 0), 0));
    //     x+=4;
    // }

    // for (int i = 30; i < 32; ++i)
    // {
    //     for (int j = 30; j < 32; ++j)
    //     {
    //         Game::units.push_back(new Unit(this, UnitConfigs::meta_units.at("laoda"), 1, QVector3D(i, j, 0), i + j));
    //     }
    // }
    run();
}

Game::~Game()
{
    pause();
    for (auto unit : units)delete unit;
    for (auto projectile : projectiles)delete projectile;
    for (auto effect : effects)delete effect;
    for (auto flock : flocks)delete flock;
}

void Game::add_projectile(Projectile* projectile)
{
    Game::projectiles.push_back(projectile);
}

void Game::add_effect(Effect* effect)
{
    Game::effects.push_back(effect);
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

void Game::run()
{
    // if (timer.isActive())
    timer_doer->start = QTime::currentTime();
    timer.start(0, timer_doer);
}

void Game::run_map_triggers()
{
    for (auto& map_event : map_config.map_events) map_event.second->update();
    for (auto& map_action : map_config.map_actions) map_action->update(map_config.map_events);
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
    run_map_triggers();
    audio_manager.play(battle_field_widget->camera_pos);
    warfare_fog_manager.draw();
    battle_field_widget->render();
}
