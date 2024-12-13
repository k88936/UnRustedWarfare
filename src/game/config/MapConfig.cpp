//
// Created by root on 11/25/24.
//

#include "MapConfig.h"

#include <iostream>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "FlowField.h"
#include "Game.h"
#include "Tile.h"
#include "UnitConfigs.h"
#include "structures/tile_attribute.h"


MapConfig::~MapConfig()
{
    for (auto& tile : tiles)
    {
        delete tile;
    }
    for (auto map_event : map_events)
    {
        delete map_event.second;
    }
    for (auto map_action : map_actions)
    {
        delete map_action;
    }
    // for (auto map_point : map_points)
    // {
    //     delete map_point.second;
    // }
    // for (auto& unit : units_on_map)
    // {
    //     delete unit;
    // }
}

TileAttribute*& MapConfig::get_tile_attribute(const int x, const int y)
{
    return tile_attributes.at(x + 1).at(y + 1);
}
QVector3D MapConfig::vector_in_which(const QVector3D& v)
{
    return QVector3D(x_in_which(v.x()), y_in_which(v.y()), v.z());
}

int MapConfig::x_in_which(const float x)
{
    // return std::min(world_width, std::max(static_cast<int>(x + 0.5f), 0));
    return static_cast<int>(x + 0.5f);
}

int MapConfig::y_in_which(const float y)
{
    // return std::min(world_height, std::max(static_cast<int>(y + 0.5f), 0));
    return static_cast<int>(y + 0.5f);
}


const std::array<std::string, 4u> LayerStrings =
{
    std::string("Tile"),
    std::string("Object"),
    std::string("Image"),
    std::string("Group"),
};


void MapConfig::parse_layer(std::vector<Tile*>& tiles, const std::unique_ptr<tmx::Layer>& layer,
                            const float z)
{
    // static float of=0;
    const auto& tmx_tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
    if (tmx_tiles.empty())
    {
        const auto& chunks = layer->getLayerAs<tmx::TileLayer>().getChunks();
        if (chunks.empty())
        {
            qDebug() << "Layer has missing tile data\n";
        }
        else
        {
            qDebug() << "Layer has " << chunks.size() << " tile chunks.\n";
        }
    }
    else
    {
        int index = 0;
        for (auto tile : tmx_tiles)
        {
            int x = index % world_width;
            int y = world_height - index / world_width - 1;
            index++;
            tiles.push_back(new Tile(tile.ID, x, y, z));
        }
    }
}

const std::string bitmap_base = "../maps/bitmaps/";

QImage base64ToImage(const QString& base64String)
{
    QByteArray byteArray = QByteArray::fromBase64(base64String.toUtf8());
    QImage image;
    image.loadFromData(byteArray);
    return image;
}

void MapConfig::loadMap(const std::string& path)
{
    tmx::Map map;
    tiles.clear();
    index_to_name.clear();
    // units_on_map.clear();

    struct meta_unit_tile
    {
        int team;
        std::string name;
    };
    std::map<int, meta_unit_tile> meta_unit_tiles;
    if (map.load(path))
    {
        if (map.isInfinite())
        {
            throw std::runtime_error("infinte map not supported");
        }
        const auto& mapProperties = map.getProperties();
        //
        for (const auto& tileset : map.getTilesets())
        {
            std::string image_path = tileset.getImagePath();
            QImage image(QString::fromStdString(image_path));
            auto properties = tileset.getProperties();
            for (auto property : properties)
            {
                if (property.getName() == "embedded_png")
                {
                    image = base64ToImage(QString::fromStdString(property.getStringValue()));
                }
            }
            if (image.isNull())
            {
                size_t pos = image_path.find_last_of("/");
                if (pos == std::string::npos)
                {
                    pos = -1;
                }
                image = QImage(QString::fromStdString(bitmap_base + image_path.substr(pos + 1)));
                if (image.isNull())
                {
                    throw std::runtime_error("Failed to load tileset image: " + image_path);
                }
            }
            int w = image.width() / tileset.getTileSize().x;
            int h = image.height() / tileset.getTileSize().y;
            index_to_name.resize(tileset.getFirstGID() + w * h, "NONE");
            index_to_attribute.resize(tileset.getFirstGID() + w * h, GameConfig::tile_configs.at("land"));
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    std::string id = tileset.getName() + std::to_string(i * w + j);
                    int count = tileset.getFirstGID() + i * w + j;
                    index_to_name[count] = id;
                    if (tile_images.contains(id))
                    {
                        continue;
                    }
                    MetaImage metaImage((image.copy(j * tileset.getTileSize().x, i * tileset.getTileSize().y,
                                                    tileset.getTileSize().x, tileset.getTileSize().y)), 1.0, false,
                                        true, 1);
                    tile_images[id] = metaImage;
                }
            }
            for (auto& tile : tileset.getTiles())
            {
                if (tile.properties.size() == 1)
                {
                    std::string p_name = tile.properties.begin()->getName();
                    try
                    {
                        index_to_attribute[tileset.getFirstGID() + tile.ID] = GameConfig::tile_configs.at(p_name);
                    }
                    catch (std::out_of_range& e)
                    {
                        qDebug() << "attribute for tileset:" << p_name << " not found";
                        index_to_attribute[tileset.getFirstGID() + tile.ID] = GameConfig::tile_configs.at("NONE");
                    }
                }
                for (auto property : tile.properties)
                {
                    std::string p_name = property.getName();
                    if (p_name == "team")
                    {
                        meta_unit_tiles[tileset.getFirstGID() + tile.ID].team = property.getStringValue() == "none"
                                ? -1
                                : std::stoi(property.getStringValue());
                    }
                    else if (p_name == "unit")
                    {
                        meta_unit_tiles[tileset.getFirstGID() + tile.ID].name = UnitConfigs::mapped_unit_name.at(
                            property.getStringValue());

                        //TODO
                    }
                    else if (p_name == "type")
                    {
                        //TODO
                    }
                    else if (p_name == "showFog")
                    {
                        //TODO
                    }
                    else

                    {
                    }
                }
            }
        }
        // MapConfig::index_to_name[0] = "NONE";
        // for (const auto& prop : mapProperties)
        // {
        //     std::cout << "Found property: " << prop.getName() << std::endl;
        //     std::cout << "Type: " << static_cast<int>(prop.getType()) << std::endl;
        // }
        const auto& layers = map.getLayers();
        // std::cout << "Map has " << layers.size() << " layers" << std::endl;
        for (const auto& layer : layers)
        {
            if (layer->getType() == tmx::Layer::Type::Object)
            {
                std::map<std::string, std::string> solve_event_only_name;
                // std::map<std::string, std::string> solve_point_only_name;
                qDebug() << "Object";
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
                for (const auto& object : objects)
                {
                    auto name = object.getName();
                    auto type = object.getType();
                    auto rect = object.getAABB();
                    auto posA = pixel_to_world(rect.left, rect.top + rect.height);
                    auto posB = pixel_to_world(rect.left + rect.width, rect.top);
                    const auto& properties = object.getProperties();
                    if (type == "unitDetect")
                    {
                        auto event = new Trigger::UnitDetect(game, posA, posB);
                        map_events[name] = event;
                        for (const auto& prop : properties)
                        {
                            if (prop.getName() == "id")
                            {
                                solve_event_only_name[prop.getStringValue()] = name;
                            }
                            else if (prop.getName() == "team")
                            {
                                event->require_team = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "type")
                            {
                                event->require_type = prop.getStringValue();
                            }
                            else if (prop.getName() == "minUnits")
                            {
                                event->require_cnt_min = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "maxUnits")
                            {
                                event->require_cnt_max = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "delay")
                            {
                                event->delay = std::stof(prop.getStringValue()) / 1000;
                            }
                            else if (prop.getName() == "globalMessage")
                            {
                                auto action = new Trigger::Dialog(game, prop.getStringValue());
                                action->activeBy = name;
                                map_actions.push_back(action);
                            }
                            else if (prop.getName() == "info")
                            {
                                auto action = new Trigger::Info(game, prop.getStringValue());
                                action->activeBy = name;
                                map_actions.push_back(action);
                            }
                            else
                            {
                                qDebug() << "unknown property: " << prop.getName();
                            }
                        }
                    }
                    else if (type == "move")
                    {
                        Trigger::UnitDetect* event = nullptr;
                        auto action = new Trigger::UnitMove(game, posA, posB);
                        map_actions.push_back(action);
                        action->activeBy = name;
                        for (const auto& prop : properties)
                        {
                            if (prop.getName() == "target")
                            {
                                action->target = prop.getStringValue();
                            }
                            else if (prop.getName() == "activeBy")
                            {
                                action->activeBy = prop.getStringValue();
                            }
                            else if (prop.getName() == "delay")
                            {
                                if (event == nullptr)
                                {
                                    event = new Trigger::UnitDetect(game, posA, posB);
                                }
                                event->delay = std::stof(prop.getStringValue()) / 1000;
                            }
                            else if (prop.getName() == "team")
                            {
                                if (event == nullptr)
                                {
                                    event = new Trigger::UnitDetect(game, posA, posB);
                                }
                                event->require_team = std::stoi(prop.getStringValue());
                                action->require_team = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "target")
                            {
                                action->target = prop.getStringValue();
                            }
                            else if (prop.getName() == "minUnits")
                            {
                                if (event == nullptr)
                                {
                                    event = new Trigger::UnitDetect(game, posA, posB);
                                }
                                event->require_cnt_min = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "maxUnits")
                            {
                                if (event == nullptr)
                                {
                                    event = new Trigger::UnitDetect(game, posA, posB);
                                }
                                event->require_cnt_max = std::stoi(prop.getStringValue());
                            }
                            else
                            {
                                qDebug() << "unknown property: " << prop.getName();
                            }
                            if (event != nullptr)
                            {
                                map_events[name] = event;
                            }
                        }
                    }
                    else if (type == "point")
                    {
                        QVector3D mid = (posA + posB) / 2;
                        flow_fields[name][movementType::LAND] = new FlowField(
                            game, mid.x(), mid.y(), movementType::LAND);
                        flow_fields[name][movementType::AIR] = new FlowField(game, mid.x(), mid.y(), movementType::AIR);
                        flow_fields[name][movementType::WATER] = new FlowField(
                            game, mid.x(), mid.y(), movementType::WATER);
                        flow_fields[name][movementType::HOVER] = new FlowField(
                            game, mid.x(), mid.y(), movementType::HOVER);
                        flow_fields[name][movementType::SUB] = new FlowField(game, mid.x(), mid.y(), movementType::SUB);
                    }
                    else if (type == "unitAdd")
                    {
                        auto action = new Trigger::UnitAdd(game, posA);
                        map_actions.push_back(action);
                        for (auto prop : properties)
                        {
                            if (prop.getName() == "activeBy")
                            {
                                action->activeBy = prop.getStringValue();
                            }
                            else if (prop.getName() == "delay")
                            {
                                action->delay = std::stof(prop.getStringValue()) / 1000;
                            }
                            else if (prop.getName() == "team")
                            {
                                action->team = std::stoi(prop.getStringValue());
                            }
                            else if (prop.getName() == "spawnUnits")
                            {
                                utils::parse_item_list(prop.getStringValue(), action->units);
                            }
                            else
                            {
                                qDebug() << "unknown property: " << prop.getName();
                            }
                        }
                    }
                    else
                    {
                        qDebug() << "unknown object type: " << type;
                    }
                }

                for (auto& map_action : map_actions)
                {
                    if (solve_event_only_name.contains(map_action->activeBy))
                    {
                        map_action->activeBy = solve_event_only_name[map_action->activeBy];
                    }
                    // if (auto* move = dynamic_cast<Trigger::Move*>(map_action))
                    // {
                    //     if (solve_point_only_name.contains(move->target))
                    //     {
                    //         move->target = solve_point_only_name[move->target];
                    //     }
                    // }
                }
            }


            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                if (layer->getName() == "Ground")
                {
                    MapConfig::world_width = layer->getSize().x;
                    MapConfig::world_height = layer->getSize().y;
                    parse_layer(tiles, layer, GameConfig::LayerConfig::TILE_GROUND);
                    const auto& tmx_tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                    tile_attributes.resize(world_width + 2);
                    for (auto& passable : tile_attributes)
                    {
                        passable.resize(world_height + 2, GameConfig::tile_configs.at("NONE"));
                    }
                    // return;
                    for (int i = 0; i < world_height; ++i)
                    {
                        for (int j = 0; j < world_width; ++j)
                        {
                            auto tmx_tile = tmx_tiles.at(i * world_width + j);
                            try
                            {
                                get_tile_attribute(j, world_height - i - 1) = index_to_attribute.at(tmx_tile.ID);
                                // std::cout << std::setfill('-') << std::setw(3) << index_to_atbute.at(tmx_tile.ID)->
                                //     type << ' ';
                                // if (index_to_attribute.at(tmx_tile.ID)->type == 8)
                                // {
                                //     std::cout << "l ";
                                // }
                                // else if (index_to_attribute.at(tmx_tile.ID)->type == 4)
                                // {
                                //     std::cout << "c ";
                                // }
                                // else
                                // {
                                //     std::cout << "w ";
                                // }
                            }
                            catch (const std::out_of_range& e)
                            {
                                qDebug() << "undefined passage for: " << index_to_name.at(tmx_tile.ID);
                            }
                        }
                        // std::cout << std::endl;
                    }
                }
                else if (layer->getName() == "Items")
                {
                    parse_layer(tiles, layer, GameConfig::LayerConfig::TILE_ITEM);
                }
                else if (layer->getName() == "Units")
                {
                    std::vector<tmx::TileLayer::Tile> tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();

                    int index = 0;
                    for (auto tile : tiles)
                    {
                        int x = index % world_width;
                        int y = world_height - index / world_width - 1;
                        index++;

                        if (tile.ID == 0)continue;
                        game->units.push_back(new Unit(
                            game, UnitConfigs::meta_units.at(meta_unit_tiles.at(tile.ID).name),
                            meta_unit_tiles.at(tile.ID).team, QVector3D(x, y, GameConfig::LayerConfig::OBJECT_GROUND),
                            0));
                    }
                }
            }
            // const auto& properties = layer->getProperties();
            // std::cout << properties.size() << " Layer Properties:" << std::endl;
            // for (const auto& prop : properties)
            // {
            //     std::cout << "Found property: " << prop.getName() << std::endl;
            //     std::cout << "Type: " << int(prop.getType()) << std::endl;
            // }
        }
    }
    else
    {
        std::cout << "Failed loading map" << std::endl;
    }
}

void MapConfig::init(Game* game, const std::string& path)
{
    this->game = game;
    loadMap(path);
}

QVector3D MapConfig::pixel_to_world(float pix_x, float pix_y)
{
    return QVector3D(pix_x / 20, world_height - pix_y / 20, 0);
}

// Tile*