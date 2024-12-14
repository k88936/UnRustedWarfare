//
// Created by root on 12/14/24.
//

#include "Arm.h"

#include "Game.h"
#include "MetaArm.h"
#include "utils.h"

Arm::Arm(Game* game, MetaArm* meta): Object(game, 1, 1, 1), meta(meta)
{
}

void Arm::before()
{
}

void Arm::step()
{
}

void Arm::after()
{
    this->relative_rotation += meta->spin_speed * game->delta_time;
    utils::angle_ensure(this->relative_rotation);
    draw(game);
}

void Arm::draw(Game* game)
{
    // color=QVector4D(1,1,1,1);
    // game->var_transparent_image_draw_config_map[meta->image].push_back(this);

    render_transform.setToIdentity();
    render_transform.translate(utils::set_offset_z(position, GameConfig::LayerConfig::SHELL));
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(scale);
    color = QVector4D(1, 1, 1, 1);
    game->var_transparent_image_draw_config_map[meta->image].push_back(this);
}
