//
// Created by root on 11/5/24.
//

#include "Unit.h"

#include "Arm.h"
#include "Controller.h"
#include "MapConfig.h"
#include "MeleeSensor.h"
#include "MetaArm.h"
#include "Sensor.h"
#include "SimpleEffect.h"
#include "UnitConfigs.h"
#include "utils.h"


Unit::Unit(Game* game, MetaUnit* meta, int team, const QVector3D position, const float rotation): Attachable(),
    Drawable(),
    Object(game, meta->radius, meta->mass, meta->mass),
    animation_moving(meta->animation_moving_start, meta->animation_idle_end, meta->animation_moving_speed),
    animation_idle(meta->animation_idle_start, meta->animation_idle_end, meta->animation_idle_speed)
{
#ifdef DEBUG
    assert(mass>0);
    // assert(radius>0);
#endif

    this->meta = meta;
    this->hp = meta->max_hp;
    this->position = position;
    this->rotation = rotation;
    this->scale = meta->scale;
    this->team = team;
    restitution = 0.8;
    linear_damping_dir = 0.25;
    linear_damping_ver = 0.4;
    angular_damping = 40;
    sight = new Sensor(game, meta->fog_of_war_sight_range, team);
    if (meta->isMelee)
        melee = new MeleeSensor(game, meta->meleeEngangementDistance, this);
    selection->scale = 1.5 * meta->radius;
    watchers.push_back(sight);
    for (const auto& slot : meta->attached_turret)
    {
        auto turret = new Turret(game, slot, team);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets.push_back(turret);
    }
    for (auto attached_arm : meta->attached_arm)
    {
        auto arm = new Arm(game, attached_arm);
        arm->slot_translation = attached_arm->slot_translation;
        arm->slot_isFixed = attached_arm->slot_isFixed;
        arm->slot_inVisible = attached_arm->slot_inVisible;
        arms.push_back(arm);
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
    if (controller)
    {
        delete controller;
    }
    if (melee)delete melee;
    delete this->shadow;
    delete this->selection;
    //remember to solve count reference
    //TODO
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
        slot->linear_velocity = this->linear_velocity;
        utils::angle_ensure_r(slot->rotation);
        slot->updateSlots(push_this);
    }
    for (auto slot : this->arms)
    {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot->slot_translation);
        slot->position = transform.map(slot->slot_translation);
        slot->rotation = slot->relative_rotation + rotation;
        slot->linear_velocity = this->linear_velocity;
        utils::angle_ensure_r(slot->rotation);
        slot->updateSlots(push_this);
    }
    for (const auto watcher : watchers)
    {
        watcher->position = this->position;
        watcher->linear_velocity = this->linear_velocity;
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

void Unit::draw(Game* game)
{
    render_transform.setToIdentity();

    if (is_driving)
    {
        render_transform.translate(this->position + utils::generate_random_small_vector(0.008));
        frame_id = animation_moving.get_value(game->delta_time);
    }
    else
    {
        render_transform.translate(this->position);
        frame_id = animation_idle.get_value(game->delta_time);
        if (utils::freq_bool(meta->effect_on_idle_chance, game->delta_time))
        {
            for (const auto effect_on_idle : meta->effect_on_idle)
            {
                game->add_effect(new Effect(game, UnitConfigs::meta_effects.at(effect_on_idle), position, rotation,
                                            linear_velocity));
            }
        }
        {
        }
    }
    shadow->render_transform = render_transform;

    render_transform.rotate(rotation, 0, 0, 1);

    selection->render_transform = render_transform;
    selection->render_transform.scale(selection->scale);

    render_transform.scale(this->scale);
    game->var_solid_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);

    shadow->render_transform.translate(-0.06, -0.06, GameConfig::LayerConfig::BOTTOM_EFFECT_OFFSET);
    shadow->render_transform.rotate(rotation, 0, 0, 1);
    shadow->render_transform.scale(this->scale);
    shadow->color = QVector4D(0, 0, 0, 0.6);
    game->var_transparent_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(shadow);
}

void Unit::drive(const QVector3D& force, const float torque)
{
    if ((force.lengthSquared() > this->mass * this->mass * 0.01 || torque != 0))
        is_driving = true;
    Object::apply_force(force, torque);
}


void Unit::before()
{
    is_driving = false;
    game->grids_manager.update_object(this);
    if (utils::team::is_allied(0, team))
    {
        game->warfare_fog_manager.light(sight, meta->fog_of_war_sight_range);
    }
    Object::before();
    for (const auto& watcher : watchers)
    {
        watcher->before();
    }
    for (const auto turret : turrets)
    {
        turret->before();
    }

    if (!this->isAttached)
    {
        QMatrix4x4 transform;
        transform.translate(this->position);
        transform.rotate(this->rotation, 0, 0, 1);
        updateSlots(transform);
    }
    this->position.setZ(0);
    if (melee)melee->before();
    if (controller) controller->before();
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
    if (melee)melee->step();
    if (controller)controller->step();
    in_sight = game->warfare_fog_manager.in_light(this);
}

void Unit::on_death()
{
    if (meta->image_wreak == utils::EMPTY_STR)
        game->add_effect(new SimpleEffect(game, meta->image, 1000,
                                          utils::add_offset_z(position, GameConfig::LayerConfig::BOTTOM_EFFECT_OFFSET),
                                          rotation,
                                          scale, linear_velocity,
                                          0, QVector4D(0.1, 0.1, 0.1, 1), true));
    else
        game->add_effect(new SimpleEffect(game, meta->image_wreak, 1000,
                                          utils::add_offset_z(position, GameConfig::LayerConfig::BOTTOM_EFFECT_OFFSET),
                                          rotation,
                                          scale, linear_velocity,
                                          0, QVector4D(1, 1, 1, 1), true));

    for (const auto& effect_on_death : meta->effect_on_death)
    {
        game->add_effect(new Effect(game, UnitConfigs::meta_effects.at(effect_on_death), position, rotation,
                                    linear_velocity));
    }
}

void Unit::on_drive()
{
    if (utils::freq_bool(2, game->delta_time))
    {
        const std::string id = utils::random_element(meta->sound_on_move, utils::EMPTY_STR);
        game->audio_manager.sound_event_config_map[id].emplace_back(
            this->position, 0.2);
    }
}

void Unit::on_new_selection()
{
    const std::string id = utils::random_element(meta->sound_on_new_selection, utils::EMPTY_STR);
    game->audio_manager.sound_event_config_map[id].emplace_back(
        this->position, 3);
}

void Unit::on_move_order()
{
    const std::string id = utils::random_element(meta->sound_on_move_order, utils::EMPTY_STR);
    game->audio_manager.sound_event_config_map[id].emplace_back(
        this->position, 3);
}

void Unit::apply_control()
{
    if (melee && melee->has_target)
    {
        melee->after();
        return;
    }
    if (controller)
    {
        controller->after();
        return;
    }
    if (meta->is_bio && utils::freq_bool(0.25, game->delta_time))
    {
        drive(vector_dir * (utils::generate_random_small_float(1) + 0.5) * mass * meta->move_acc * 1,
              utils::generate_random_small_float(1) * inertia * meta->turn_acc * 1);
    }
}

void Unit::after()

{
    // if (std::isnan(this->position.x()))
    //     throw std::runtime_error("nan");
    if (this->marked_for_delete)
    {
    }
    for (const auto& watcher : watchers)
    {
        watcher->after();
    }

    apply_control();

    for (const auto turret : turrets)
    {
        turret->after();
    }
    for (auto arm : arms)
    {
        arm->after();
    }
    utils::linear_limit_max_soft_r(linear_velocity, meta->move_speed, 0.8);
    utils::linear_limit_max_soft_r(angular_velocity, meta->max_turn_speed, 0.8);
    Object::after();
    const int tile_x = game->map_config.x_in_which(position.x());
    int tile_y = game->map_config.y_in_which(position.y());
    if (!(game->map_config.get_tile_attribute(tile_x, tile_y)->type & this->meta->movement))
    {
        linear_velocity *= 0.1;
    }
    else
    {
        constexpr float soft = 0.8;
        constexpr float space = 0.4f;
        if (!(game->map_config.get_tile_attribute(tile_x - 1, tile_y)->type & this->meta->movement))
        {
            position.setX(utils::linear_limit_soft_v(position.x(), tile_x - space, std::numeric_limits<float>::max(),
                                                     soft));
            linear_velocity.setX(
                utils::linear_limit_soft_v(linear_velocity.x(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(game->map_config.get_tile_attribute(tile_x + 1, tile_y)->type & this->meta->movement))
        {
            position.setX(
                utils::linear_limit_soft_v(position.x(), -std::numeric_limits<float>::max(), tile_x + space, soft));
            linear_velocity.setX(
                utils::linear_limit_soft_v(linear_velocity.x(), -std::numeric_limits<float>::max(), 0.0f, soft));
        }
        if (!(game->map_config.get_tile_attribute(tile_x, tile_y - 1)->type & this->meta->movement))
        {
            position.setY(utils::linear_limit_soft_v(position.y(), tile_y - space, std::numeric_limits<float>::max(),
                                                     soft));
            linear_velocity.setY(
                utils::linear_limit_soft_v(linear_velocity.y(), 0.0f, std::numeric_limits<float>::max(), soft));
        }
        if (!(game->map_config.get_tile_attribute(tile_x, tile_y + 1)->type & this->meta->movement))
        {
            position.setY(
                utils::linear_limit_soft_v(position.y(), -std::numeric_limits<float>::max(), tile_y + space, soft));
            linear_velocity.setY(
                utils::linear_limit_soft_v(linear_velocity.y(), -std::numeric_limits<float>::max(), 0.0f, soft));
        }
    }


    if (is_driving)
    {
        on_drive();
    }

    if (!marked_for_delete && hp <= 0)
    {
        if (tags.contains("__protect__"))game->battle_field_widget->game_end(false);
        on_death();
        marked_for_delete = true;
    }
    if (in_sight)
        this->draw(game);
}

void Unit::on_collision(const QVector3D& force, float torque, Object* other)
{
    if (this->meta->is_bio && utils::team::is_enemy(team, other->team) && force.lengthSquared() > mass * mass * 4000)
    {
        this->hp -= 10;
    }
    Object::on_collision(force, torque, other);
}
