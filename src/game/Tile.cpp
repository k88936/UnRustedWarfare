//
// Created by root on 11/25/24.
//

#include "Tile.h"

#include <Game.h>
#include <MapConfig.h>

Tile::Tile(const int index, const int x, const int y,const float z)
{
    this->index = index;
    this->x = x;
    this->y = y;
    this->render_transform.translate(x,y,z);
}

void Tile::draw()
{
    Game::const_image_draw_config_map[MapConfig::index_to_name.at(index)].push_back(this);
}
