//
// Created by root on 11/18/24.
//

#include "Effect.h"

#include "Game.h"
#include "utils.h"


Effect::Effect(Game* game): ::Object(game, 1, 1, 1)
{
}

Effect::Effect(Game* game, MetaEffect* meta, const QVector3D position, const float rotation,
               const QVector3D& linear_velocity_base): ::Object(game, 1, 1, 1)
{
    this->meta = meta;
    this->position = position;
    if (meta->draw_under_units)
    {
        this->position.setZ(GameConfig::LayerConfig::BOTTOM_EFFECT_OFFSET);
    }
    if (meta->attached_to_unit)
    {
        this->linear_velocity = linear_velocity_base + QVector3D(meta->x_speed_relative, meta->y_speed_relative,
                                                                 -meta->h_speed - 0.1) +
            utils::generate_random_small_vector((meta->x_speed_relative_random + meta->y_speed_relative_random) / 2);
    }
    else
    {
        this->linear_velocity = QVector3D(meta->x_speed_absolute, meta->y_speed_absolute, -meta->h_speed - 0.01) +
            utils::generate_random_small_vector((meta->x_speed_relative_random + meta->y_speed_relative_random) / 2);
    }
    // if (meta->x_speed_absolute != 0 || meta->y_speed_absolute != 0)
    // {
    //     this->linear_velocity = linear_velocity_base + QVector3D(meta->x_speed_absolute, meta->y_speed_absolute, 0);
    // }
    // else
    // {
    //     this->linear_velocity = linear_velocity_base + QVector3D(
    //         meta->x_speed_relative_random * (std::rand() % 100) / 100 + meta->x_speed_relative,
    //         meta->y_speed_relative_random * (std::rand() % 100) / 100 + meta->y_speed_relative, 0);
    // }

    if (meta->physics)
    {
        restitution = 0.8;
        linear_damping_dir = 2.5;
        linear_damping_ver = 2.5;
        angular_damping = 40;
    }
    this->color = meta->color;
    this->color.setZ(meta->alpha);
    this->rotation = rotation;
    this->scale = meta->scale_from;
    this->animate_timer = 0;


    game->audio_manager.sound_event_config_map[meta->also_play_sound].emplace_back(
        this->position, meta->also_play_sound_volume);
}

void Effect::draw(Game* game)
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    game->var_transparent_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
}

void Effect::before()
{
    Object::before();
}


void Effect::step()
{
    in_sight = game->warfare_fog_manager.in_light(this);
}

void Effect::after()
{
    Object::after();
    this->scale = meta->scale_from + (meta->scale_to - meta->scale_from) * (has_life) / meta->life;
    animate_timer += game->delta_time;
    if (meta->fade_out && has_life > meta->life - meta->fade_in_time)
    {
        this->color.setZ(meta->alpha * (meta->life - has_life) / meta->fade_in_time);
    }
    else if (has_life < meta->fade_in_time)
    {
        this->color.setZ(meta->alpha * has_life / meta->fade_in_time);
    }

    if ((this->has_life += game->delta_time) >= meta->life)
    {
        marked_for_delete = true;
    };

    if (meta->animate_frame_start != meta->animate_frame_end)
    {
        if (animate_timer > meta->animate_frame_delay)
        {
            animate_timer -= meta->animate_frame_delay;
            frame_id++;
            if (frame_id > meta->animate_frame_end)
            {
                marked_for_delete = true;
                return;
            }
        }
    }

    if (in_sight)
        draw(game);
}
