//
// Created by root on 12/2/24.
//

#include "BoidSensor.h"

#include "utils.h"

#include <numbers>

#include "Flock.h"
#include "FlowField.h"

BoidSensor::BoidSensor(Unit* unit): BoidSensor(unit->game, 4, unit)
{
}

BoidSensor::~BoidSensor()
{
    if (flock != nullptr)
    {
        flock->boids.erase(unit_under_control);
    }
}


void BoidSensor::before()
{
    position = unit_under_control->position;
    linear_velocity = unit_under_control->linear_velocity;
    if (flock == nullptr)
    {
        return Sensor::before();
    }
    nearby_count = 1;
    angular_target = 0;
    speed_target *= 0;
    nearby_even_rotation = unit_under_control->rotation;
    nearby_even_speed = unit_under_control->linear_velocity;
    nearby_even_center = unit_under_control->position;
}

void BoidSensor::step()
{
    if (flock == nullptr)
    {
        return Sensor::step();
    }
    const int x_floor = static_cast<int>(unit_under_control->position.x());
    const int y_floor = static_cast<int>(unit_under_control->position.y());
    const int x_ceil = x_floor + 1;
    const int y_ceil = y_floor + 1;
    if (flock->flow_field != nullptr)
    {
        QVector3D flow_field_expected;
        if (arrived_flock_target)
        {
            flow_field_expected = target_offset - unit_under_control->position;
        }
        else
        {
            // flow_field_expected=flock->flow_field->get_vector(boid->position.x()+0.5, boid->position.y()+0.5);
            flow_field_expected = QVector3D(
                (flock->flow_field->get_vector(x_floor, y_floor).x() + flock->flow_field->
                                                                              get_vector(x_floor, y_ceil).x()) * (x_ceil
                    -
                    unit_under_control
                    ->
                    position.x())
                + (flock->flow_field->get_vector(x_ceil, y_floor).x() + flock->flow_field->
                                                                               get_vector(x_ceil, y_ceil).x()) * (
                    unit_under_control->
                    position.
                    x() - x_floor),
                (flock->flow_field->get_vector(x_floor, y_floor).y() + flock->flow_field->
                                                                              get_vector(x_ceil, y_floor).y()) * (y_ceil
                    -
                    unit_under_control
                    ->
                    position.y())
                + (flock->flow_field->get_vector(x_floor, y_ceil).y() + flock->flow_field->
                                                                               get_vector(x_ceil, y_ceil).y()) * (
                    unit_under_control->
                    position.
                    y() - y_floor),
                0
            );
        }


        // qDebug()<<flow_field_expected.lengthSquared();
        if (utils::within(this->position, flock->pos_target, flock->arrived_range))
        {
            this->arrived_flock_target = true;
        }
        if (utils::within(this->position, target_offset, 0.6))
        {
            this->arrived_flock_target_offset = true;
        }

        if (this->arrived_flock_target_offset)
        {
            if (!utils::within(this->position, target_offset, 1))
            {
                this->arrived_flock_target_offset = false;
            }
        }
        else
        {
            if (flow_field_expected.lengthSquared() > 0.1)
            {
                speed_target += 0.8f * unit_under_control->meta->move_speed * flow_field_expected.normalized();
            }
        }
    }
}

void BoidSensor::after()

{
    speed_target += nearby_even_speed / nearby_count * 0.2f;
    if (this->arrived_flock_target_offset)
    {
        angular_target += unit_under_control->rotation * 0.8f;
    }
    else
    {
        angular_target += utils::dir_of(speed_target) * 0.8f;
    }
    if (flock == nullptr)
    {
        return Sensor::after();
    }
    angular_target += 0.2 * nearby_even_rotation / nearby_count;
    utils::angle_ensure(angular_target);


    float speed_projected = QVector3D::dotProduct(unit_under_control->linear_velocity, unit_under_control->vector_dir);
    float angle_step = unit_under_control->meta->max_turn_speed;

    if (unit_under_control->meta->is_wheel_powered)
    {
        angle_step *= (speed_projected) / unit_under_control->meta->move_speed;
    }
    float diff = angular_target - unit_under_control->rotation;
    utils::angle_ensure(diff);
    if (unit_under_control->meta->moveIgnoringBody)
    {
    }
    else if (std::fabsf(diff) < angle_step * game->delta_time)
    {
        // unit0->rotation = target;
        unit_under_control->angular_velocity *= 0.8;
        can_drive = true;
    }
    else
    {
        if (unit_under_control->meta->is_wheel_powered)
        {
            can_drive = true;
        }
        else
        {
            if (std::fabsf(diff) < angle_step * game->delta_time * 4)
            {
                can_drive = true;
            }
            else
            {
                can_drive = false;
            }
        }
        const float acc = unit_under_control->meta->turn_acc * unit_under_control->inertia;
        unit_under_control->drive(
            unit_under_control->vector_ver * speed_projected * unit_under_control->mass * unit_under_control->
            angular_velocity * std::numbers::pi / 180,
            utils::sign(diff) * acc);
        utils::linear_limit_soft_r(unit_under_control->angular_velocity, angle_step * game->delta_time,
                                   -angle_step * game->delta_time,
                                   0.8);
    }


    // speed_target *= 0.99;
    if (arrived_flock_target_offset)
    {
        if (!utils::within(this->position, target_offset, 0.2))
        {
            unit_under_control->drive(
                -unit_under_control->mass * unit_under_control->linear_velocity * unit_under_control->meta->move_dec,
                0);
        }
    }
    else if (unit_under_control->meta->moveIgnoringBody)
    {
        unit_under_control->drive(

            speed_target.normalized() * unit_under_control->meta->move_acc * unit_under_control->mass, 0
        );
    }
    else if (can_drive)
    {
        float speed_target_length = speed_target.length();
        if (speed_projected > 0)
        {
            if (speed_target_length > speed_projected)
            {
                unit_under_control->drive(
                    unit_under_control->mass * unit_under_control->vector_dir * unit_under_control->meta->move_acc,
                    0);
            }
            else
            {
                unit_under_control->drive(
                    -unit_under_control->mass * unit_under_control->vector_dir * unit_under_control->meta->move_dec,
                    0);
            }
        }
        else if (speed_projected < 0)
        {
            if (speed_target_length > speed_projected)
            {
                unit_under_control->drive(
                    unit_under_control->mass * unit_under_control->vector_dir * unit_under_control->meta->move_dec,
                    0);
            }
            else
            {
                unit_under_control->drive(
                    -unit_under_control->mass * unit_under_control->vector_dir * unit_under_control->meta->move_acc,
                    0);
            }
        }
        else
        {
            unit_under_control->drive(
                unit_under_control->mass * unit_under_control->vector_dir * unit_under_control->meta->move_acc,
                0);
        }
    }
}

bool BoidSensor::on_overlay(Object* obj, const QVector3D position_diff)
{
    if (flock == nullptr)
    {
        return Sensor::on_overlay(obj, position_diff);
    }
    if (auto unit = dynamic_cast<Unit*>(obj))
    {
        if (flock->boids.contains(unit))
        {
            this->nearby_even_rotation += unit->rotation;
            this->nearby_even_speed += unit->linear_velocity;
            this->nearby_even_center += unit->position;
            //position
            nearby_count++;
            if (position_diff.lengthSquared() < this->radius * this->radius / 4)
            {
                this->angular_target += 10 * QVector3D::dotProduct(unit_under_control->vector_ver, position_diff) /
                    position_diff.
                    lengthSquared();
            }
        }
    }
    return Sensor::on_overlay(obj, position_diff);
}
