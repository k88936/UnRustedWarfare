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
    position=unit_under_control->position;
    has_target=false;
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
        if ((target-position).lengthSquared()>unit_under_control->radius*unit_under_control->radius)
        {
            unit_under_control->drive((target - unit_under_control->position).normalized()*unit_under_control->mass*unit_under_control->meta->move_acc, 0);
        utils::linear_limit_max_soft_r(unit_under_control->linear_velocity,unit_under_control->meta->move_speed,0.5);
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
                target=unit->position;
                has_target = true;
            }
        }
    }
    return Sensor::on_overlay(obj, position_diff);
}
