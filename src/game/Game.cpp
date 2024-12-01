//
// Created by root on 11/10/24.
//

#include "Game.h"

#include <QObject>

#include "battle_widget.h"
#include "UnitConfigs.h"

#include "Unit.h"
#include "Effect.h"
#include "FlowField.h"
#include "MapConfig.h"
#include "PathFind.h"
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

FlowField* flowField;

void Game::init()
{
    UnitConfigs::init();
    MapConfig::init();
    MapConfig::loadMap("../maps/2.tmx");
    //
    // MapConfig::world_width=100;
    // MapConfig::world_height=100;
    grids_manager.init();
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

    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), QVector3D(30, 43, 0), -140));
    Game::units.push_back(new Unit(UnitConfigs::meta_units.at("m2a3"), QVector3D(80, 50, 0), -5));


    flowField = new FlowField(80, 50, movementType::LAND);
    // Game::line_draw_config.clear();
    //
    // for (int i = 0; i < MapConfig::world_width; ++i)
    // {
    //     for (int j = 0; j < MapConfig::world_height; ++j)
    //     {
    //         Game::line_draw_config.emplace_back(i, j, 0);
    //         Game::line_draw_config.emplace_back(flowField->get_vector(i, j) * 0.4 + QVector3D(i, j, 0));
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
    grids_manager.clear_grids();
    Game::var_image_draw_config_map.clear();
    for (const auto u : Game::units)
    {
        grids_manager.update_object(u);
    }
    for (const auto p : Game::projectiles)
    {
        grids_manager.update_object(p);
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
    // units[2]->applyForce(units[0]->vectorDir * units[0]->mass * 6, 0);
    units[1]->attack(units[0]->position);
    const auto unit0 = units[0];
    int x_floor = static_cast<int>(unit0->position.x());
    int y_floor = static_cast<int>(unit0->position.y());
    int x_ceil = x_floor + 1;
    int y_ceil = y_floor + 1;

    QVector3D expected(
        (flowField->get_vector(x_floor, y_floor).x() + flowField->get_vector(x_floor, y_ceil).x()) * (x_ceil - unit0->
            position.x())
        + (flowField->get_vector(x_ceil, y_floor).x() + flowField->get_vector(x_ceil, y_ceil).x()) * (unit0->position.
            x() - x_floor),
        (flowField->get_vector(x_floor, y_floor).y() + flowField->get_vector(x_ceil, y_floor).y()) * (y_ceil - unit0->
            position.y())
        + (flowField->get_vector(x_floor, y_ceil).y() + flowField->get_vector(x_ceil, y_ceil).y()) * (unit0->position.
            y() - y_floor),
        0
    );

    expected.normalize();
    expected *= unit0->meta->moveSpeed*3;
    unit0->applyForce(
        QVector3D::dotProduct(expected, unit0->vector_dir) * unit0->vector_dir * unit0->mass,
        QVector3D::dotProduct(expected, unit0->vector_ver) * unit0->mass * 6);

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
