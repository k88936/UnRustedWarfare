//
// Created by root on 12/14/24.
//

#include "WarfareFogManager.h"

#include "Game.h"
#include "utils.h"

const float magic = 5;

void WarfareFogManager::init()
{
    light_dense.resize(game->grids_manager.width * game->grids_manager.grid_size);
    for (auto& dense : light_dense) dense.resize(game->grids_manager.height * game->grids_manager.grid_size, 0);

    fogs.resize(game->map_config.world_height * game->map_config.world_width);
    for (int i = 0; i < game->map_config.world_width; ++i)
    {
        for (int j = 0; j < game->map_config.world_height; ++j)
        {
            fogs[i * game->map_config.world_height + j].render_transform.translate(i, j, GameConfig::LayerConfig::FOG);
            fogs[i * game->map_config.world_height + j].color = QVector4D(0, 0, 0, 0.9);
        }
    }
}


void WarfareFogManager::light(const Object* obj, const float range)
{
    for (auto grid : obj->gridsAcross)
    {
        for (int i = 0; i < game->grids_manager.grid_size; ++i)
        {
            for (int j = 0; j < game->grids_manager.grid_size; ++j)
            {
                QVector3D pos(grid->x * game->grids_manager.grid_size + i, grid->y * game->grids_manager.grid_size + j,
                              0);
                //lightness
                float light = (
                    // magic * 5+
                    range * range - (obj->position - pos).
                    lengthSquared());
                if (light > 0)
                {
                    light_dense[pos.x()][pos.y()] += light;
                }
            }
        }
    }
}

bool WarfareFogManager::in_light(Object* obj)
{
    return light_dense[game->map_config.x_in_which(obj->position.x())][game->map_config.y_in_which(obj->position.y())] >
        0;
}


void WarfareFogManager::draw()
{
    for (int i = 0; i < game->map_config.world_width; ++i)
    {
        for (int j = 0; j < game->map_config.world_height; ++j)
        {
            float dense = light_dense[i][j];
            if (dense <= 0)
            {
                game->var_transparent_image_draw_config_map["_fog_4"].push_back(
                    &fogs[i * game->map_config.world_height + j]);
            }
            else if (dense < 1 * magic)
            {
                game->var_transparent_image_draw_config_map["_fog_3"].push_back(
                    &fogs[i * game->map_config.world_height + j]);
            }
            else if (dense < 2 * magic)
            {
                game->var_transparent_image_draw_config_map["_fog_2"].push_back(
                    &fogs[i * game->map_config.world_height + j]);
            }
            else if (dense < 3 * magic)
            {
                game->var_transparent_image_draw_config_map["_fog_1"].push_back(
                    &fogs[i * game->map_config.world_height + j]);
            }
            else if (dense < 4 * magic)
            {
                game->var_transparent_image_draw_config_map["_fog_0"].push_back(
                    &fogs[i * game->map_config.world_height + j]);
            }
        }
    }
    for (auto& dense : light_dense)
    {
        for (auto& d : dense)
        {
            d = 0;
        }
    }
}
