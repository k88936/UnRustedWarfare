//
// Created by root on 11/25/24.
//

#ifndef MAPCONFIG_H
#define MAPCONFIG_H
#include <iosfwd>
#include <string>
#include <vector>
#include <vector>
#include <tmxlite/Layer.hpp>

#include "Tile.h"
#include "structures/meta_image.h"
#include "structures/tile_attribute.h"


class MapConfig
{
public:
    static std::vector<std::unique_ptr<Tile>> tiles;
    static int world_width;
    static int world_height;
    static const std::map<std::string, TileAttribute*> tile_configs;
    static TileAttribute*& get_tile_attribute(int x, int y);
    static int x_in_which(float x);
    static int y_in_which(float y);
    static void init();
    static void config_layer(std::vector<std::unique_ptr<Tile>>& tiles, const std::unique_ptr<tmx::Layer>& layer,
                             float z);
    static void loadMap(const std::string& path);
    static std::map<std::string, MetaImage> tile_images;
    static std::vector<std::string> index_to_name;

private:
    static std::vector<std::vector<TileAttribute*>> tile_attributes;
};


#endif //MAPCONFIG_H
