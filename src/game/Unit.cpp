//
// Created by root on 11/5/24.
//

#include "Unit.h"
#include "BoidSensor.h"
#include "Game.h"
#include "MapConfig.h"
#include "Sensor.h"
#include "utils.h"


Unit::Unit(MetaUnit* meta, const QVector3D position, const float rotation): Attachable(), Drawable(),
                                                                            Object(meta->radius, meta->mass, meta->mass)
{
    this->meta = meta;
    this->position = position;
    this->rotation = rotation;
    this->scale = meta->scale;
    for (const auto& slot : meta->attached_turret)
    {
        auto turret = new Turret(slot);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets.push_back(turret);
    }
    auto* mar = new Sensor(meta->maxAttackRange);
    watchers.push_back(mar);

    boid_sensor = new BoidSensor(4, this);
    watchers.push_back(boid_sensor);
    restitution = 0.8;
    linearDampingDir = 0.8;
    linearDampingVer = 1.2;
    angularDamping = 40;
}

void Unit::updateSlots(QMatrix4x4 transform)
{
    transform.rotate(this->relative_rotation, 0, 0, 1);
    for (auto slot : this->turrets)
    {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot->slot_translation);
        slot->position = transform.map(slot->slot_translation);
        slot->rotation=slot->relative_rotation+rotation;
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
    for (const auto& slot : turrets)
    {
        if (slot->slot_inVisible)
        {
            continue;
        }
        slot->draw();
    }
}


void Unit::before()
{
    for (const auto& watcher : watchers)
    {
        watcher->before();
    }
    Object::before();
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
}


void Unit::after()


{
    for (const auto& watcher : watchers)
    {
        watcher->after();
    }
    constexpr float space = 0.2f;
    constexpr float soft = 0.5;
    Object::after();
    const int tile_x = MapConfig::x_in_which(position.x());
    int tile_y = MapConfig::y_in_which(position.y());
    if (!(MapConfig::get_tile_attribute(tile_x, tile_y)->type & this->meta->movement))
    {
        linearVelocity *= 0.1;
    }
    else
    {
        if (!(MapConfig::get_tile_attribute(tile_x - 1, tile_y)->type & this->meta->movement))
        {
            position.setX(utils::limit_soft_v(position.x(), tile_x - space, std::numeric_limits<float>::max(), soft));
            linearVelocity.setX(utils::limit_soft_v(linearVelocity.x(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x + 1, tile_y)->type & this->meta->movement))
        {
            position.setX(
                utils::limit_soft_v(position.x(), std::numeric_limits<float>::min(), tile_x + 1 + space, soft));
            linearVelocity.setX(utils::limit_soft_v(linearVelocity.x(), std::numeric_limits<float>::min(), 0.0f, soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x, tile_y - 1)->type & this->meta->movement))
        {
            position.setY(utils::limit_soft_v(position.y(), tile_y - space, std::numeric_limits<float>::max(), soft));
            linearVelocity.setY(utils::limit_soft_v(linearVelocity.y(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(MapConfig::get_tile_attribute(tile_x, tile_y + 1)->type & this->meta->movement))
        {
            position.setY(
                utils::limit_soft_v(position.y(), std::numeric_limits<float>::min(), tile_y + 1 + space, soft));
            linearVelocity.setY(utils::limit_soft_v(linearVelocity.y(), std::numeric_limits<float>::min(), 0.0f, soft));
        }
    }

    if (!this->isAttached)
    {
        QMatrix4x4 transform;
        transform.translate(this->position);
        transform.rotate(this->rotation, 0, 0, 1);
        updateSlots(transform);
    }
    this->draw();
}
