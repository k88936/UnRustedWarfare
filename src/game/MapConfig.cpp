//
// Created by root on 11/25/24.
//

#include "MapConfig.h"

#include <iostream>
#include <tmxlite/LayerGroup.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include "Tile.h"

std::vector<MetaImage> MapConfig::tile_images;
std::vector<std::string> MapConfig::index_to_name;

const std::array<std::string, 4u> LayerStrings =
{
    std::string("Tile"),
    std::string("Object"),
    std::string("Image"),
    std::string("Group"),
};

void MapConfig::init()
{
}

void refill(int n)
{
    MapConfig::tile_images.resize(n);
    MapConfig::index_to_name.resize(n);
}

void add()
{
}

void MapConfig::loadMap(const std::string& path, std::vector<Tile*>& tiles)
{
    tmx::Map map;

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
            std::string path = tileset.getImagePath();
            QImage image(QString::fromStdString(path));

            int w = image.width() / tileset.getTileSize().x;
            int h = image.height() / tileset.getTileSize().y;
            refill(tileset.getFirstGID() + w * h);
            for (int i = 0; i < h; i++)
            {
                for (int j = 0; j < w; j++)
                {
                    int count = tileset.getFirstGID() + i * w + j;
                    QImage img = image.copy(j * tileset.getTileSize().x, i * tileset.getTileSize().y,
                                            tileset.getTileSize().x, tileset.getTileSize().y);
                    MetaImage metaImage(img, 1, false, true,1);
                    MapConfig::tile_images[count] = metaImage;
                    MapConfig::index_to_name[count] = tileset.getName() + std::to_string(i * w + j);
                }
            }
        }
        MapConfig::index_to_name[0] = "";

        for (const auto& prop : mapProperties)
        {
            std::cout << "Found property: " << prop.getName() << std::endl;
            std::cout << "Type: " << static_cast<int>(prop.getType()) << std::endl;
        }

        std::cout << std::endl;

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
                std::cout << "Checking sublayers" << std::endl;
                const auto& sublayers = layer->getLayerAs<tmx::LayerGroup>().getLayers();
                std::cout << "LayerGroup has " << sublayers.size() << " layers" << std::endl;
                for (const auto& sublayer : sublayers)
                {
                    std::cout << "Found Layer: " << sublayer->getName() << std::endl;
                    std::cout << "Sub-layer Type: " << LayerStrings[static_cast<std::int32_t>(sublayer->getType())] <<
                        std::endl;
                    std::cout << "Sub-layer Class: " << sublayer->getClass() << std::endl;
                    std::cout << "Sub-layer Dimensions: " << sublayer->getSize() << std::endl;
                    std::cout << "Sub-layer Tint: " << sublayer->getTintColour() << std::endl;

                    if (sublayer->getType() == tmx::Layer::Type::Object)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::ObjectGroup>().
                            getObjects().size() << " objects" << std::endl;
                    }
                    else if (sublayer->getType() == tmx::Layer::Type::Tile)
                    {
                        std::cout << sublayer->getName() << " has " << sublayer->getLayerAs<tmx::TileLayer>().getTiles()
                                                                               .size() << " tiles" << std::endl;
                    }
                }
            }

            if (layer->getType() == tmx::Layer::Type::Object)
            {
                const auto& objects = layer->getLayerAs<tmx::ObjectGroup>().getObjects();
                std::cout << "Found " << objects.size() << " objects in layer" << std::endl;
                for (const auto& object : objects)
                {
                    std::cout << "Object " << object.getUID() << ", " << object.getName() << std::endl;
                    const auto& properties = object.getProperties();
                    std::cout << "Object has " << properties.size() << " properties" << std::endl;
                    for (const auto& prop : properties)
                    {
                        std::cout << "Found property: " << prop.getName() << std::endl;
                        std::cout << "Type: " << int(prop.getType()) << std::endl;
                    }

                    if (!object.getTilesetName().empty())
                    {
                        std::cout << "Object uses template tile set " << object.getTilesetName() << "\n";
                    }
                }
            }

            if (layer->getType() == tmx::Layer::Type::Tile)
            {
                const auto& tmx_tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
                if (tmx_tiles.empty())
                {
                    const auto& chunks = layer->getLayerAs<tmx::TileLayer>().getChunks();
                    if (chunks.empty())
                    {
                        std::cout << "Layer has missing tile data\n";
                    }
                    else
                    {
                        std::cout << "Layer has " << chunks.size() << " tile chunks.\n";
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
                            tiles.push_back(new Tile(tmx_tiles[i * width + j].ID, j, height - i));
                        }
                        std::cout << std::endl;
                    }

                    std::cout << "Layer has " << tmx_tiles.size() << " tiles.\n";
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
