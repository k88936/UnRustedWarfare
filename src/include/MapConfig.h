//
// Created by root on 11/25/24.
//

#ifndef MAPCONFIG_H
#define MAPCONFIG_H
#include <iosfwd>
#include <map>
#include <string>
#include <vector>
#include <tmxlite/Layer.hpp>

#include "Tile.h"
#include "structures/meta_image.h"
#include "structures/tile_attribute.h"


class MapConfig
{
public:
    std::vector<Tile*> tiles;
    int world_width;
    int world_height;
   static  const std::map<std::string, TileAttribute*> tile_configs;
    TileAttribute*& get_tile_attribute(int x, int y);
    int x_in_which(float x);
    int y_in_which(float y);
    void init();
    void config_layer(std::vector<Tile*>& tiles, const std::unique_ptr<tmx::Layer>& layer,
                      float z);
    void loadMap(const std::string& path);
    std::map<std::string, MetaImage> tile_images;
    std::vector<std::string> index_to_name;
    std::vector<std::vector<TileAttribute*>> tile_attributes;
};


#endif //MAPCONFIG_H
