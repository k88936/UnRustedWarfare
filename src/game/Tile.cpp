//
// Created by root on 11/25/24.
//

#include "Tile.h"

#include <Game.h>
#include <MapConfig.h>

Tile::Tile(int index, int x, int y)
{
    this->index = index;
    this->x = x;
    this->y = y;
    this->render_transform.translate(x,y,-1);
}

void Tile::draw()
{
    Game::const_image_draw_config_map[MapConfig::index_to_name[index]].push_back(this);
}
