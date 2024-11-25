//
// Created by root on 11/10/24.
//

#include "Game.h"

#include <QObject>

#include "UnitConfigs.h"

#include "Unit.h"
#include "Effect.h"
#include "MapConfig.h"
#include "Projectile.h"
#include "Tile.h"
#include "welcome_widget.h"

float Game::deltaTime = 1.0 / 60;
float Game::FPS = 1.0 / deltaTime;
std::unordered_map<std::string, std::vector<Drawable *> > Game::var_image_draw_config_map;
std::unordered_map<std::string, std::vector<Drawable *> > Game::const_image_draw_config_map;
GridsManager Game::grids_manager;
std::vector<Unit *> Game::units;
std::vector<Tile*> Game::tiles;
QBasicTimer Game::timer;
std::vector<Projectile *> Game::projectiles;
std::vector<Effect *> Game::effects;
BattlefieldWidget *Game::battleFieldWidget;


void Game::addProjectile(Projectile *projectile) {
    Game::projectiles.push_back(projectile);
}

void Game::addEffect(Effect* effect)
{
    Game::effects.push_back(effect);
}

    welcome_widget* welcome;
void Game::init() {
    UnitConfigs::init();
    MapConfig::init();
    MapConfig::loadMap("../maps/2.tmx",tiles);
    // battleFieldWidget->show();
    welcome = new welcome_widget();
    battleFieldWidget = new BattlefieldWidget(welcome);



    for (auto tile : tiles)
    {
        tile->draw();
    }



    // welcome->setWidget( battleFieldWidget);
    welcome->show();
    class TimerDoer : public QObject {
        QOBJECT_H
    public:
        void timerEvent(QTimerEvent *event) override {
            Game::step();
        };
    };
    timer.start(static_cast<int>(deltaTime * 1000), new TimerDoer());
    Game::units.push_back( new Unit(UnitConfigs::meta_units["m2a3"], QVector3D(-4, 1, 0), 10));
    Game::units.push_back(new Unit(UnitConfigs::meta_units["m2a3"], QVector3D(4, 0.7, 0), 150));
}

void Game::clean()
{
    for(auto it = Game::projectiles.begin(); it != Game::projectiles.end();)
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
    for(auto it = Game::effects.begin(); it != Game::effects.end();)
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
    for(auto it = Game::units.begin(); it != Game::units.end();)
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

void Game::step() {
    grids_manager.clearGrids();
    Game::var_image_draw_config_map.clear();
    for (const auto u: Game::units) {
        grids_manager.updateObject(u);
    }
    for (const auto p: Game::projectiles) {
        grids_manager.updateObject(p);
    }
    for (const auto u: Game::units) {
        u->before();
    }
    for (const auto p: Game::projectiles) {
        p->before();
    }
    for (const auto & e : Game::effects)
    {
        e->before();
    }
    // units[0]->applyForce(QVector3D(units[0]->mass * 1, 0, 0), 0);
    units[0]->applyForce(units[0]->vectorDir*units[0]->mass*6, 0);
    units[1]->attack(units[0]->position);
    // units[1]->attack(units[0]->position);
    for (const auto u: Game::units) {
        u->step();
    }
    for (const auto u: Game::units) {
        u->after();
    }
    for (const auto p: Game::projectiles) {
        p->after();
    }
    for (const auto & e : Game::effects)
    {
        e->after();
    }
    clean();
    welcome->update();
    // battleFieldWidget->update();
}
