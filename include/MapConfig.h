//
// Created by root on 11/25/24.
//

#ifndef MAPCONFIG_H
#define MAPCONFIG_H
#include <string>
#include <Structures.h>
#include <vector>

#include "Tile.h"

class MapConfig {

public:
    static void init();
    static void loadMap(const ::std::string& path, std::vector<Tile*>& tiles);
    static std::vector<MetaImage> tile_images;
    static std::vector<std::string> index_to_name;
};



#endif //MAPCONFIG_H
