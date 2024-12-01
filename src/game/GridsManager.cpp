//
// Created by root on 11/14/24.
//

#include "GridsManager.h"

#include <iostream>
#include "MapConfig.h"

#include "Object.h"

void GridsManager::clear_grids() const
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            grids[i][j]->objects.clear();
        }
    }
}

void GridsManager::init()
{
    this->width = MapConfig::world_width - 1 / grid_size + 1;
    this->height = MapConfig::world_height - 1 / grid_size + 1;
    //fill
    grids.resize(width);
    for (int i = 0; i < width; ++i)
    {
        grids[i].resize(height);
        for (int j = 0; j < height; ++j)
        {
            grids[i][j] = new Grid();
            // std::cout << grids[i][j];
        }
    }
}

GridsManager::GridsManager(): grid_size(4)
{
}

int GridsManager::x_in_which(const float x) const
{
    return std::min(width, std::max(static_cast<int>((x + 0.5f) / grid_size), 0));
}

int GridsManager::y_in_which(const float y) const
{
    return std::min(height, std::max(static_cast<int>((y + 0.5f )/ grid_size), 0));
}

// grid *SpaceEngine::whichIn(const float x, const float y) const {
//     return grids[static_cast<int>(x) / grid_size][static_cast<int>(y) / grid_size];
// }
//
// void SpaceEngine::whichAcross(const float x, const float y, const float r, std::vector<grid *> &gridsAcross) const {
//     gridsAcross.clear();
//
//     int x1 = static_cast<int>(x - r / grid_size);
//     const int x2 = static_cast<int>(x + r / grid_size);
//     int y1 = static_cast<int>(y - r / grid_size);
//     const int y2 = static_cast<int>(y + r / grid_size);
//     while (x1 <= x2) {
//         while (y1 <= y2) {
//             gridsAcross.push_back(grids[x1][y1]);
//             y1++;
//         }
//         x1++;
//     }
// }


void GridsManager::update_object(Object* object) const
{
    object->gridsAcross.clear();
    int x1 = x_in_which(object->position.x() - object->radius);
    int x2 = x_in_which(object->position.x() + object->radius);
    int y1 = y_in_which(object->position.y() - object->radius);
    int y2 = y_in_which(object->position.y() + object->radius);

    // int x1 =static_cast<int> ((object->position.x() - object->radius) / grid_size);
    // int x2=static_cast<int>((object->position.x()+object->radius)/grid_size);
    // int y1 =static_cast<int> ((object->position.y() - object->radius) / grid_size);
    // int y2=static_cast<int>((object->position.y()+object->radius)/grid_size);
    // std::cout<<object<<std::endl;
    int i = x1;
    while (i <= x2)
    {
        int j = y1;
        while (j <= y2)
        {
            if (i < 0 || j < 0 || i >= width || j >= height)
            {
                j++;
                continue;
            }
            grids[i][j]->objects.push_back(object);
            object->gridsAcross.push_back(grids[i][j]);
            j++;
        }
        i++;
    }
    // std::cout.flush();
}
