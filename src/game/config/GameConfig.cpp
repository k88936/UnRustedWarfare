//
// Created by root on 12/10/24.
//

#include "GameConfig.h"
#include "structures/tile_attribute.h"

float GameConfig::game_speed = 1;
float GameConfig::game_volume = 0.5;

/**
 * land cliff water sub
 */
const std::map<std::string, TileAttribute*> GameConfig::tile_configs = {
    {"NONE", new TileAttribute(0b0000, 0)},
    {"land", new TileAttribute(0b1000, 1)},
    {"large-rock", new TileAttribute(0b0100, 1)},
    {"large-cliff", new TileAttribute(0b0100, 1)},
    {"lava", new TileAttribute(0b0000, 1)},
    {"lava-cliff", new TileAttribute(0b0100, 1)},
    {"cliff", new TileAttribute(0b0100, 1.1)},
    {"water", new TileAttribute(0b0010, 1.2)},
    {"water-bridge", new TileAttribute(0b1010, 1.2)},
    {"water-cliff", new TileAttribute(0b0010, 1.3)},
    // {"sub", new TileAttribute(0b0001, 1.4)},
    // {"res_pool", new TileAttribute(0b1000, 1.4)},
    // {"res_pool", new TileAttribute(0b1000, 1.4)},
};
