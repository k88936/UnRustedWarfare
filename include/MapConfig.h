//
// Created by root on 11/25/24.
//

#ifndef MAPCONFIG_H
#define MAPCONFIG_H
#include <string>
#include <Structures.h>
#include <vector>
#include <tmxlite/Layer.hpp>

#include "Tile.h"

class MapConfig {

public:
    static void init();
    static void config_layer(std::vector<std::unique_ptr<Tile>>& tiles, const std::unique_ptr<tmx::Layer>& layer);
    static void loadMap(const std::string& path, std::vector<std::unique_ptr<Tile>>& tiles);
    static std::map<std::string,MetaImage> tile_images;
    static std::vector<std::string> index_to_name;
};



#endif //MAPCONFIG_H
