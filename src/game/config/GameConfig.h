//
// Created by root on 12/10/24.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include <map>
#include <string>


struct TileAttribute;

class GameConfig {
public:
    static float  game_speed;
    static float  game_volume;
    static const std::map<std::string, TileAttribute*> tile_configs;
};



#endif //GAMECONFIG_H
