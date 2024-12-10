//
// Created by root on 11/14/24.
//

#include "Sensor.h"

#include "Game.h"


Sensor::Sensor(Game *game,const float radius,int team): Object(game,radius,0,0) {

    this->team=team;
}

void Sensor::before()
{
    game->grids_manager.update_object(this);
    Object::before();
}

void Sensor::step()
{
    Object::step();
}

void Sensor::after()
{
}

bool Sensor::on_overlay(Object *obj, const QVector3D position_diff) {
    return false;
}
