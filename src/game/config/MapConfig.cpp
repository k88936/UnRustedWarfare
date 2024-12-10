//
// Created by root on 11/25/24.
//

#include "MapConfig.h"

#include <iostream>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "Game.h"
#include "Tile.h"
#include "structures/tile_attribute.h"

MapConfig::MapConfig(const std::string& path)
{
    loadMap(path);
}

MapConfig::~MapConfig()
{
    for (auto& tile : tiles)
    {
        delete tile;
    }
}

TileAttribute*& MapConfig::get_tile_attribute(const int x, const int y)
{
    return tile_attributes.at(x + 1).at(y + 1);
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


void MapConfig::config_layer(std::vector<Tile*>& tiles, const std::unique_ptr<tmx::Layer>& layer,
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
        int width = layer->getSize().x;
        int height = layer->getSize().y;
        for (int i = 0; i < height; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // std::cout << " " << tmx_tiles.at(i * width + j).ID;
                tiles.push_back(new Tile(tmx_tiles.at(i * width + j).ID, j, height - i - 1, z));
                // of-=0.00001;
            }
        }
        std::cout << "Layer has " << tmx_tiles.size() << " tiles.\n";
    }
}

const std::string bitmap_base = "../maps/bitmaps/";

void MapConfig::loadMap(const std::string& path)
{
    tmx::Map map;
    tiles.clear();
    index_to_name.clear();
    if (map.load(path))
    {
        // std::cout << "Loaded Map version: " << map.getVersion().upper << ", " << map.getVersion().lower << std::endl;
        if (map.isInfinite())
        {
            // std::cout << "Map is infinite.\n";
        }
        else
        {
            // std::cout << "Map Dimensions: " << map.getBounds() << std::endl;
        }

        const auto& mapProperties = map.getProperties();
        // std::cout << "Map class: " << map.getClass() << std::endl;

        // std::cout << "Map tileset has " << map.getTilesets().size() << " tilesets" << std::endl;
        for (const auto& tileset : map.getTilesets())
        {
            std::string image_path = tileset.getImagePath();
            // if (image_path.starts_with("ridges/"||image_path.starts_with("ridges/")))
            // {
            //     image_path = image_path.substr()
            // }
            QImage image(QString::fromStdString(image_path));

            if (image.isNull())
            {
                size_t pos = image_path.find_last_of("/\\");
                if (pos == std::string::npos)
                {
                    throw std::runtime_error("Failed to load tileset image: " + image_path);
                }
                image = QImage(QString::fromStdString(bitmap_base + image_path.substr(pos + 1)));
                if (image.isNull())
                {
                    throw std::runtime_error("Failed to load tileset image: " + image_path);
                }
            }
            int w = image.width() / tileset.getTileSize().x;
            int h = image.height() / tileset.getTileSize().y;
            MapConfig::index_to_name.resize(tileset.getFirstGID() + w * h, "NONE");
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    std::string id = tileset.getName() + std::to_string(i * w + j);
                    int count = tileset.getFirstGID() + i * w + j;
                    MapConfig::index_to_name[count] = id;
                    if (tile_images.contains(id))
                    {
                        continue;
                    }
                    MetaImage metaImage((image.copy(j * tileset.getTileSize().x, i * tileset.getTileSize().y,
                                                    tileset.getTileSize().x, tileset.getTileSize().y)), 1.025, false,
                                        true, 1);
                    MapConfig::tile_images[id] = metaImage;
                }
            }
        }
        // MapConfig::index_to_name[0] = "NONE";
        for (const auto& prop : mapProperties)
        {
            std::cout << "Found property: " << prop.getName() << std::endl;
            std::cout << "Type: " << static_cast<int>(prop.getType()) << std::endl;
        }
        const auto& layers = map.getLayers();
        std::cout << "Map has " << layers.size() << " layers" << std::endl;
        for (const auto& layer : layers)
        {
            std::cout << "Found Layer: " << layer->getName() << std::endl;
            std::cout << "Layer Type: " << LayerStrings[static_cast<std::int32_t>(layer->getType())] << std::endl;
            std::cout << "Layer Dimensions: " << layer->getSize() << std::endl;
            std::cout << "Layer Tint: " << layer->getTintColour() << std::endl;

            if (layer->getType() == tmx::Layer::Type::Group)
            {
                // std::cout << "Checking sublayers" << std::endl;
                // const auto& sublayers = layer->getLayerAs<tmx::LayerGroup>().getLayers();
                // std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
                // for (const auto& sublayer : sublayers)
                // {
                //     std::cout << "Found Layer: " << sublayer->getName() << std::endl;
                //     std::cout << "Sub-layer Type: " << LayerStrings[static_cast<std::int32_t>(sublayer->getType())] <<
                //         std::endl;
                //     std::cout << "Sub-layer Class: " << sublayer->getClass() << std::endl;
                //     std::cout << "Sub-layer Dimensions: " << sublayer->getSize() << std::endl;
                //     std::cout << "Sub-layer Tint: " << sublayer->getTintColour() << std::endl;
                //
                //     if (sublayer->getType() == tmx::Layer::Type::Object)
                //     {
                //         std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::ObjectGroup>().
                //             getObjects().size() << " objects" << std::endl;
                //     }
                //     else if (sublayer->getType() == tmx::Layer::Type::Tile)
                //     {
                //         std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::TileLayer>().getTiles()
                //                                                                .size() << " tiles" << std::endl;
                //     }
                // }
            }

            if (layer->getType() == tmx::Layer::Type::Object)
            {
                // const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
                // std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
                // for (const auto& object : objects)
                // {
                //     std::cout << "Object " << object.getUID() << ", " << object.getName() << std::endl;
                //     const auto& properties = object.getProperties();
                //     std::cout << "Object has " << properties.size() << " properties" << std::endl;
                //     for (const auto& prop : properties)
                //     {
                //         std::cout << "Found property: " << prop.getName() << std::endl;
                //         std::cout << "Type: " << int(prop.getType()) << std::endl;
                //     }
                //
                //     if (!object.getTilesetName().empty())
                //     {
                //         std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
                //     }
                // }
            }


            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                if (layer->getName() == "Ground")
                {
                    config_layer(tiles, layer, Game::LayerConfig::TILE_GROUND);
                }
                else if (layer->getName() == "Items")
                {
                    config_layer(tiles, layer, Game::LayerConfig::TILE_ITEM);
                }
                else if (layer->getName() == "Units")
                {
                    qDebug() << "Units";
                }
                else if (layer->getName() == "set")
                {
                    qDebug() << "set";
                    const auto& tmx_tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                    MapConfig::world_width = layer->getSize().x;
                    MapConfig::world_height = layer->getSize().y;
                    tile_attributes.resize(world_width + 2);
                    for (auto& passable : tile_attributes)
                    {
                        passable.resize(world_height + 2, GameConfig::tile_configs.at("None"));
                    }
                    // return;
                    for (int i = 0; i < world_height; ++i)
                    {
                        for (int j = 0; j < world_width; ++j)
                        {
                            auto tmx_tile = tmx_tiles.at(i * world_width + j);
                            try
                            {
                                get_tile_attribute(j, world_height - i - 1) = GameConfig::tile_configs.
                                    at(index_to_name.at(tmx_tile.ID));
                            }
                            catch (const std::out_of_range& e)
                            {
                                qDebug() << "undefined passage for: " << index_to_name.at(tmx_tile.ID);
                            }
                        }
                    }
                }
            }
            const auto& properties = layer->getProperties();
            std::cout << properties.size() << " Layer Properties:" << std::endl;
            for (const auto& prop : properties)
            {
                std::cout << "Found property: " << prop.getName() << std::endl;
                std::cout << "Type: " << int(prop.getType()) << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Failed loading map" << std::endl;
    }
}
