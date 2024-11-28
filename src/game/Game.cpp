//
// Created by root on 11/10/24.
//

#include "Game.h"

#include <QObject>

#include "battle_widget.h"
#include "UnitConfigs.h"

#include "Unit.h"
#include "Effect.h"
#include "MapConfig.h"
#include "Projectile.h"
#include "Tile.h"
#include "welcome_widget.h"

float Game::deltaTime = 1.0 / 40;
float Game::FPS = 1.0 / deltaTime;

std::vector<QVector3D> Game::line_draw_config;
std::unordered_map<std::string, std::vector<Drawable*>> Game::var_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable*>> Game::const_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable*>> Game::ui_image_draw_config_map;
GridsManager Game::grids_manager;
std::vector<Unit*> Game::units;
QBasicTimer Game::timer;
std::vector<Projectile*> Game::projectiles;
std::vector<Effect*> Game::effects;
BattlefieldWidget* Game::battleFieldWidget;

void Game::addProjectile(Projectile* projectile)
{
    Game::projectiles.push_back(projectile);
}

void Game::addEffect(Effect* effect)
{
    Game::effects.push_back(effect);
}

void Game::init()
{
    UnitConfigs::init();
    MapConfig::init();
    MapConfig::loadMap("../maps/2.tmx");
    // battleFieldWidget->show();
    // welcome = new welcome_widget();
    // battleFieldWidget = welcome->get_battleFieldWidget();
    // welcome->show();
    auto battle = new battle_widget();
    battleFieldWidget = battle->get_battleFieldWidget();
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

    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), QVector3D(120, 120, 0), -135));
    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), QVector3D(115, 120, 0), -140));
    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), QVector3D(120, 115, 0), -130));
    for (int i = 0; i < 50; ++i)
    {
        for (int j = 0; j < 50; ++j)
        {
    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("laoda"), QVector3D(i*2+1, 2*j+1, 0), 150));
        }
    }
    // units[0]->applyForce(units[0]->vectorDir * units[0]->mass * 600, 0);
    // units[1]->applyForce(units[0]->vectorDir * units[0]->mass * 600, 0);
    // units[2]->applyForce(units[0]->vectorDir * units[0]->mass * 600, 0);
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
        if ((*it)->marked_for_delete)
        {
            delete *it;
            it = Game::units.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::step()
{
    // qDebug()<<"step begin";
    grids_manager.clearGrids();
    Game::var_image_draw_config_map.clear();
    for (const auto u : Game::units)
    {
        grids_manager.updateObject(u);
    }
    for (const auto p : Game::projectiles)
    {
        grids_manager.updateObject(p);
    }
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
    // units[0]->applyForce(QVector3D(units[0]->mass * 1, 0, 0), 0);
    // units[0]->applyForce(units[0]->vectorDir * units[0]->mass * 6, 0);
    // units[1]->applyForce(units[0]->vectorDir * units[0]->mass * 6, 0);
    // units[2]->applyForce(units[0]->vectorDir * units[0]->mass * 6, 0);
    // units[1]->attack(units[0]->position);
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
    clean();
    // welcome->update();

    battleFieldWidget->update();
}
