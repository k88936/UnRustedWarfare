//
// Created by root on 11/5/24.
//

#include "Unit.h"
#include "BoidSensor.h"
#include "Game.h"
#include "MapConfig.h"
#include "Sensor.h"
#include "utils.h"


Unit::Unit(MetaUnit* meta, int team, const QVector3D position, const float rotation): Attachable(), Drawable(),
    Object(meta->radius, meta->mass, meta->mass)
{
    this->meta = meta;
    this->position = position;
    this->rotation = rotation;
    this->scale = meta->scale;
    this->team = team;
    restitution = 0.8;
    linear_damping_dir = 0.8;
    linear_damping_ver = 1.2;
    angular_damping = 40;
    auto* mar = new Sensor(meta->max_attack_range, team);
    watchers.push_back(mar);
    boid_sensor = new BoidSensor(4, this);
    watchers.push_back(boid_sensor);
    for (const auto& slot : meta->attached_turret)
    {
        auto turret = new Turret(slot, team);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets.push_back(turret);
    }
}

Unit::~Unit()
{
    for (const auto turret : turrets)
    {
        delete turret;
    }
    for (const auto watcher : watchers)
    {
        delete watcher;
    }
}

void Unit::updateSlots(QMatrix4x4 transform)
{
    transform.rotate(this->relative_rotation, 0, 0, 1);
    for (auto slot : this->turrets)
    {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot->slot_translation);
        slot->position = transform.map(slot->slot_translation);
        slot->rotation = slot->relative_rotation + rotation;
        utils::angle_ensure(slot->rotation);
        slot->updateSlots(push_this);
    }
    for (const auto watcher : watchers)
    {
        watcher->position = this->position;
    }
}

// void Unit::attack(QVector3D target) {
//     for (const auto &slot: turrets) {
//         float diff = slot->aim(target);
//         if (diff == 0) {
//             slot->shoot();
//         }
//     }
// }

void Unit::attack(const QVector3D& target)
{
    for (const auto turret : turrets)
    {
        turret->attack(target);
    }
}

void Unit::draw()
{
    render_transform.setToIdentity();
    // if (this->linearVelocity.lengthSquared() == 0 && this->angularVelocity == 0)
    // {
    //     render_transform.translate(this->position);
    // }
    // else
    // {
    //     render_transform.translate(position + generateRandomSmallVector(0.025));
    // }
    render_transform.translate(this->position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    Game::var_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
    // for (const auto& slot : turrets)
    // {
    //     if (slot->slot_inVisible)
    //     {
    //         continue;
    //     }
    //     slot->draw();
    // }
}


void Unit::before()
{
    Game::grids_manager.update_object(this);
    for (const auto& watcher : watchers)
    {
        watcher->before();
    }
    for (const auto turret : turrets)
    {
        turret->preferred_target = this->prefered_target;
        turret->before();
    }
    Object::before();
    if (!this->isAttached)
    {
        QMatrix4x4 transform;
        transform.translate(this->position);
        transform.rotate(this->rotation, 0, 0, 1);
        updateSlots(transform);
    }
    this->position.setZ(0);
    // this->position.setZ(meta->targetHeight);
}

void Unit::step()
{
    Object::step();
    for (const auto& watcher : watchers)
    {
        watcher->step();
    }
    for (auto turret : turrets)
    {
        turret->step();
    }
}


void Unit::after()
{
    for (const auto& watcher : watchers)
    {
        watcher->after();
    }
    for (const auto turret : turrets)
    {
        turret->after();
    }
    constexpr float space = 0.4f;
    constexpr float soft = 0.8;
    Object::after();
    const int tile_x = MapConfig::x_in_which(position.x());
    int tile_y = MapConfig::y_in_which(position.y());
    if (!(MapConfig::get_tile_attribute(tile_x, tile_y)->type & this->meta->movement))
    {
        linear_velocity *= 0.1;
    }
    else
    {
        if (!(MapConfig::get_tile_attribute(tile_x - 1, tile_y)->type & this->meta->movement))
        {
            position.setX(utils::linear_limit_soft_v(position.x(), tile_x - space, std::numeric_limits<float>::max(), soft));
            linear_velocity.setX(utils::linear_limit_soft_v(linear_velocity.x(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x + 1, tile_y)->type & this->meta->movement))
        {
            position.setX(
                utils::linear_limit_soft_v(position.x(), std::numeric_limits<float>::min(), tile_x  + space, soft));
            linear_velocity.setX(utils::linear_limit_soft_v(linear_velocity.x(), std::numeric_limits<float>::min(), 0.0f, soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x, tile_y - 1)->type & this->meta->movement))
        {
            position.setY(utils::linear_limit_soft_v(position.y(), tile_y - space, std::numeric_limits<float>::max(), soft));
            linear_velocity.setY(utils::linear_limit_soft_v(linear_velocity.y(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x, tile_y + 1)->type & this->meta->movement))
        {
            position.setY(
                utils::linear_limit_soft_v(position.y(), std::numeric_limits<float>::min(), tile_y + space, soft));
            linear_velocity.setY(utils::linear_limit_soft_v(linear_velocity.y(), std::numeric_limits<float>::min(), 0.0f, soft));
        }
    }


    this->draw();
}
