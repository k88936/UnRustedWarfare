//
// Created by root on 12/5/24.
//

#include "SimpleEffect.h"

#include <utility>

#include "Game.h"

SimpleEffect::SimpleEffect(std::string image, const float life, const QVector3D position, const float rotation,
                           const float scale,
                           const QVector3D linear_velocity, const float angular_velocity): max_life(life),
    image(std::move(image))
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->linear_velocity = linear_velocity;
    this->angular_velocity = angular_velocity;

    restitution = 0.8;
    linear_damping_dir = 0.8;
    linear_damping_ver = 1.2;
    angular_damping = 40;
}

void SimpleEffect::draw()
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    color = QVector4D(0.1, 0.1, 0.1, 1);
    Game::var_image_draw_config_map[image].push_back(this);
}

void SimpleEffect::before()
{
    this->position.setZ(-0.1);
    Effect::before();
}

void SimpleEffect::after()
{
    Object::after();
    if (has_life >= max_life)
    {
        marked_for_delete = true;
    }
    has_life += Game::deltaTime;
    draw();
}
