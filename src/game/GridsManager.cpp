//
// Created by root on 11/14/24.
//

#include "GridsManager.h"

#include <iostream>

#include "Game.h"
#include "MapConfig.h"
#include "Object.h"
#include "Unit.h"


void GridsManager::clear_grids() const
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            // for (auto object : grids[i][j]->objects)
            // {
            //
            // }
            grids[i][j]->objects.clear();
        }
    }
}

void GridsManager::init(Game* game)
{
    this->width = game->map_config.world_width - 1 / grid_size + 1;
    this->height = game->map_config.world_height - 1 / grid_size + 1;
    //fill
    grids.resize(width);
    for (int i = 0; i < width; ++i)
    {
        grids[i].resize(height);
        for (int j = 0; j < height; ++j)
        {
            grids[i][j] = new Grid(i,j);
            // std::cout << grids[i][j];
        }
    }
}

GridsManager::GridsManager(): grid_size(4)
{
}

GridsManager::~GridsManager()
{
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            delete grids[i][j];
        }
    }
}

int GridsManager::x_in_which(const float x) const
{
    return std::min(width, std::max(static_cast<int>((x + 0.5f) / grid_size), 0));
}

int GridsManager::y_in_which(const float y) const
{
    return std::min(height, std::max(static_cast<int>((y + 0.5f) / grid_size), 0));
}

std::set<Unit*> GridsManager::scan_units(QVector3D pos_lb, QVector3D pos_rt)
{
    int x1 = x_in_which(pos_lb.x());
    int x2 = x_in_which(pos_rt.x());
    int y1 = y_in_which(pos_lb.y());
    int y2 = y_in_which(pos_rt.y());
    std::set<Unit*> units;
    for (int i = x1; i <= x2; ++i)
    {
        for (int j = y1; j <= y2; ++j)
        {
            auto objects_in_grid = grids[i][j]->objects;

            for (auto obj : objects_in_grid)
            {
                if (auto unit = dynamic_cast<Unit*>(obj); unit &&
                    obj->position.x() > pos_lb.x() && obj->position.x() < pos_rt.x() &&
                    obj->position.y() > pos_lb.y() && obj->position.y() < pos_rt.y()
                )
                {
                    units.insert(unit);
                }
            }
        }
    }
    return units;
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
    for (int i = x1; i <= x2; ++i)
    {
        for (int j = y1; j <= y2; ++j)
        {
            grids[i][j]->objects.push_back(object);
            object->gridsAcross.push_back(grids[i][j]);
        }
    }
    // std::cout.flush();
}


std::vector<Grid*> GridsManager::scan_grids(const QVector3D& pos, const float radius)
{
    int x1 = x_in_which(pos.x() - radius);
    int x2 = x_in_which(pos.x() + radius);
    int y1 = y_in_which(pos.y() - radius);
    int y2 = y_in_which(pos.y() + radius);
    std::vector<Grid*> grids_across;
    for (int i = x1; i <= x2; ++i)
    {
        for (int j = y1; j <= y2; ++j)
        {
            grids_across.push_back(grids[i][j]);
        }
    }
    return grids_across;
}

std::set<Unit*> GridsManager::scan_units(float pos_x, float pos_y, float width, float height)
{
    int x1 = x_in_which(pos_x);
    int x2 = x_in_which(pos_x + width);
    int y1 = y_in_which(pos_y);
    int y2 = y_in_which(pos_y + height);
    std::set<Unit*> units;
    for (int i = x1; i <= x2; ++i)
    {
        for (int j = y1; j <= y2; ++j)
        {
            auto objects_in_grid = grids[i][j]->objects;

            for (auto obj : objects_in_grid)
            {
                if (auto unit = dynamic_cast<Unit*>(obj); unit &&
                    obj->position.x() > pos_x && obj->position.x() < pos_x + width &&
                    obj->position.y() > pos_y && obj->position.y() < pos_y + height
                )
                {
                    units.insert(unit);
                }
            }
        }
    }
    return units;
}
