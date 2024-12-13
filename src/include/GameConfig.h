//
// Created by root on 12/10/24.
//

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
#include <map>
#include <string>


struct TileAttribute;

class GameConfig
{
public:
    static float game_speed;
    static float game_volume;
    static const std::map<std::string, TileAttribute*> tile_configs;

    class LayerConfig
    {
    public:
        constexpr static float TILE_GROUND = -1;
        constexpr static float TILE_ITEM = -0.9;
        constexpr static float OBJECT_GROUND = 0;
        constexpr static float TURRET_OFFSET = 0.1;
        constexpr static float BOTTOM_EFFECT_OFFSET = -0.05;
        constexpr static float SHELL = 1;
        constexpr static float OBJECT_AIR = 2;
        constexpr static float UPPER_EFFECT_OFFSET = 0.05;
      constexpr  static   float FOG=2.5;
    };
};


#endif //GAMECONFIG_H
