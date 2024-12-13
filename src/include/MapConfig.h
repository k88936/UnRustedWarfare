//
// Created by root on 11/25/24.
//

#ifndef MAPCONFIG_H
#define MAPCONFIG_H
#include <map>
#include <string>
#include <vector>
#include <tmxlite/Layer.hpp>

#include "Tile.h"
#include "structures/meta_image.h"
#include "structures/tile_attribute.h"


class FlowField;

namespace Trigger
{
    class Action;
    class Event;
}

class Unit;

class MapConfig
{
public:
    ~MapConfig();
    std::vector<Tile*> tiles;
    int world_width;
    int world_height;
    // std::vector<Unit*> units_on_map;
    Game* game;
    TileAttribute*& get_tile_attribute(int x, int y);
    int x_in_which(float x);
    int y_in_which(float y);
    void parse_layer(std::vector<Tile*>& tiles, const std::unique_ptr<tmx::Layer>& layer,
                     float z);
    void loadMap(const std::string& path);
    void init(Game* game, const std::string& path);
    QVector3D pixel_to_world(float pix_x, float pix_y);
    std::map<std::string, MetaImage> tile_images;
    std::vector<std::string> index_to_name;
    std::vector<TileAttribute*> index_to_attribute;
    std::map<std::string, Trigger::Event*> map_events;
    std::vector<Trigger::Action*> map_actions;
    std::vector<std::vector<TileAttribute*>> tile_attributes;
    std::map<std::string,FlowField* > flow_fields;

    std::map<std::string,FlowField*> map_points;
    // Warning all is copies of tile_configs, forbidden to delete
};


#endif //MAPCONFIG_H
