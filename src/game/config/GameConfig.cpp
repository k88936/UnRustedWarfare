//
// Created by root on 12/10/24.
//

#include "GameConfig.h"
#include "structures/tile_attribute.h"

float GameConfig::game_speed=1;
float GameConfig::game_volume=0.5;
const std::map<std::string, TileAttribute*> GameConfig::tile_configs = {
    {"None", new TileAttribute(0b0000, 0)},
    // {"Grass0", new TileAttribute(0b0001, 1)},
    // {"Water0", new TileAttribute(0b0010, 1)},
    {"Shallow Water0", new TileAttribute(0b0100, 1)},
    {"Long Grass0", new TileAttribute(0b1000, 1)},
    {"Sand0", new TileAttribute(0b1000, 1)},
    {"Mountain0", new TileAttribute(0b1000, 1.1)},
    {"Ice0", new TileAttribute(0b1000, 1.2)},
    {"Snow0", new TileAttribute(0b1000, 1.4)},
};

