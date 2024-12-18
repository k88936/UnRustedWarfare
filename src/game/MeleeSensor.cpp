//
// Created by root on 12/17/24.
//

#include "MeleeSensor.h"

#include "Unit.h"

MeleeSensor::MeleeSensor(Game* game, const float radius, Unit* unit): Sensor(game, radius, unit->team), Controller(unit)
{
#ifdef DEBUG
    assert(unit->meta->isMelee);
#endif
}

void MeleeSensor::before()
{
    position = unit_under_control->position;
    has_target = false;
    Sensor::before();
}

void MeleeSensor::step()
{
    Sensor::step();
}

void MeleeSensor::after()
{
    if (has_target)
    {
        if (QVector3D relative = target - unit_under_control->position; relative.lengthSquared() > unit_under_control->radius * unit_under_control->radius)
        {
            const float diff = utils::angle_ensure_v(utils::dir_of(relative) - unit_under_control->rotation);
            const float angle_step = unit_under_control->meta->max_turn_speed;
            float acc = 0;
            if (std::fabsf(diff) < angle_step * unit_under_control->game->delta_time)
            {
                unit_under_control->angular_velocity *= 0.8;
            }
            else
                acc = unit_under_control->meta->turn_acc * unit_under_control->inertia;
            unit_under_control->drive(
                relative.normalized() * unit_under_control->mass * unit_under_control->
                                                                   meta->move_acc,
                utils::sign(diff) * acc);
        }
    }
    return Sensor::after();
}

bool MeleeSensor::on_overlay(Object* obj, QVector3D position_diff)
{
    if (!has_target)
    {
        if (const auto unit = dynamic_cast<Unit*>(obj))
        {
            if (utils::team::is_enemy(this->team, unit->team))
            {
                target = unit->position;
                has_target = true;
            }
        }
    }
    return Sensor::on_overlay(obj, position_diff);
}
