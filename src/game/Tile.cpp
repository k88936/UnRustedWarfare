//
// Created by root on 11/25/24.
//

#include "Tile.h"

#include "Game.h"

Tile::Tile(const int index, const int x, const int y,const float z)
{
    this->index = index;
    // this->x = x;
    // this->y = y;
    this->render_transform.translate(x,y,z);
}

void Tile::draw(Game* game)
{
    game->const_image_draw_config_map[game->map_config.index_to_name.at(index)].push_back(this);
}
