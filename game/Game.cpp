//
// Created by root on 11/10/24.
//

#include "Game.h"

#include <QObject>

#include "Configs.h"
#include "Unit.h"

float Game::deltaTime = 1.0 / 60;
std::unordered_map<std::string, std::vector<Drawable *> > Game::image_draw_config_map;
GridsManager Game::grids_manager;
std::vector<Unit *> Game::units;
QBasicTimer Game::timer;
std::vector<Projectile *> Game::projectiles;
MainWidget *Game::battleFieldWidget;


void Game::init() {
    Configs::init();
    battleFieldWidget = new MainWidget();
    battleFieldWidget->show();
    class TimerDoer : public QObject {
        QOBJECT_H

    public:
        void timerEvent(QTimerEvent *event) override {
            Game::step();
        };
    };
    timer.start(static_cast<int>(deltaTime * 1000), new TimerDoer());
    Game::units.push_back( new Unit(Configs::meta_units["m2a3"], QVector3D(0, 1, 0), 80));
    Game::units.push_back(new Unit(Configs::meta_units["m2a3"], QVector3D(4, 0.7, 0), 80));
}

void Game::step() {
    grids_manager.clearGrids();
    Game::image_draw_config_map.clear();
    for (const auto u: Game::units) {
        grids_manager.updateObject(u);
    }
    for (const auto u: Game::units) {
        u->before();
    }
    units[0]->applyForce(QVector3D(units[0]->mass * 3, 0, 0), 0);
    units[1]->turrets[0]->aim(units[0]->position);
    for (const auto u: Game::units) {
        u->step();
    }
    for (const auto u: Game::units) {
        u->after();
    }
    battleFieldWidget->update();
}
