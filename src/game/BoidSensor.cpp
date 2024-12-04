//
// Created by root on 12/2/24.
//

#include "BoidSensor.h"

#include "Game.h"
#include "utils.h"

#include <numbers>

BoidSensor::~BoidSensor()
{
    if (flock != nullptr)
    {
        flock->boids.erase(boid);
    }
}

void BoidSensor::before()
{
    if (flock == nullptr)
    {
        return Sensor::before();
    }
    if (flock->preferred_target != nullptr)
    {
        boid->prefered_target = flock->preferred_target;
    }
    nearby_count = 1;
    angular_target = 0;
    speed_target *= 0;
    nearby_even_rotation = boid->rotation;
    nearby_even_speed = boid->linearVelocity;
    nearby_even_center = boid->position;
}

void BoidSensor::step()
{
    if (flock == nullptr)
    {
        return Sensor::step();
    }
    const int x_floor = static_cast<int>(boid->position.x());
    const int y_floor = static_cast<int>(boid->position.y());
    const int x_ceil = x_floor + 1;
    const int y_ceil = y_floor + 1;
    if (flock->flow_field != nullptr)
    {
        QVector3D flow_field_expected;
        if (arrived_flock_target)
        {
            flow_field_expected = target_offset - boid->position;
        }
        else
            flow_field_expected = QVector3D(
                (flock->flow_field->get_vector(x_floor, y_floor).x() + flock->flow_field->
                                                                              get_vector(x_floor, y_ceil).x()) * (x_ceil
                    -
                    boid
                    ->
                    position.x())
                + (flock->flow_field->get_vector(x_ceil, y_floor).x() + flock->flow_field->
                                                                               get_vector(x_ceil, y_ceil).x()) * (boid->
                    position.
                    x() - x_floor),
                (flock->flow_field->get_vector(x_floor, y_floor).y() + flock->flow_field->
                                                                              get_vector(x_ceil, y_floor).y()) * (y_ceil
                    -
                    boid
                    ->
                    position.y())
                + (flock->flow_field->get_vector(x_floor, y_ceil).y() + flock->flow_field->
                                                                               get_vector(x_ceil, y_ceil).y()) * (boid->
                    position.
                    y() - y_floor),
                0
            );


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
                speed_target += 0.8f * boid->meta->move_speed * flow_field_expected.normalized();
            }
        }
    }
}

void BoidSensor::after()

{
    speed_target += nearby_even_speed / nearby_count * 0.2f;
    if (this->arrived_flock_target_offset)
    {
        angular_target += boid->rotation * 0.8f;
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
    float speed_projected = QVector3D::dotProduct(boid->linearVelocity, boid->vector_dir);
    float angle_step = boid->meta->max_turn_speed;
    if (boid->meta->is_wheel_powered)
    {
        angle_step *= (speed_projected) / boid->meta->move_speed;
    }
    float diff = angular_target - boid->rotation;
    utils::angle_ensure(diff);
    if (std::fabsf(diff) < angle_step * Game::deltaTime)
    {
        // unit0->rotation = target;
        boid->angularVelocity *= 0.8;
    }
    else
    {
        const float acc = boid->meta->turn_acc * boid->inertia;
        boid->apply_force(
            boid->vector_ver * speed_projected * boid->mass * boid->angularVelocity * std::numbers::pi / 180,
            utils::sign(diff) * acc);
        utils::linear_limit_soft_r(boid->angularVelocity, angle_step * Game::deltaTime, -angle_step * Game::deltaTime, 0.8);
    }


    // speed_target *= 0.99;
    if (arrived_flock_target_offset)
    {
        boid->apply_force(-boid->mass * boid->linearVelocity * boid->meta->move_dec, 0);
    }
    else
    {
        float speed_target_length = speed_target.length();
        if (speed_projected > 0)
        {
            if (speed_target_length > speed_projected)
            {
                boid->apply_force(
                    boid->mass * boid->vector_dir * boid->meta->move_acc,
                    0);
            }
            else
            {
                boid->apply_force(
                    -boid->mass * boid->vector_dir * boid->meta->move_dec,
                    0);
            }
        }
        else if (speed_projected < 0)
        {
            if (speed_target_length > speed_projected)
            {
                boid->apply_force(
                    boid->mass * boid->vector_dir * boid->meta->move_dec,
                    0);
            }
            else
            {
                boid->apply_force(
                    -boid->mass * boid->vector_dir * boid->meta->move_acc,
                    0);
            }
        }
        else
        {
            boid->apply_force(
                boid->mass * boid->vector_dir * boid->meta->move_acc,
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
            this->nearby_even_speed += unit->linearVelocity;
            this->nearby_even_center += unit->position;
            //position
            nearby_count++;
            if (position_diff.lengthSquared() < this->radius * this->radius / 4)
            {
                this->angular_target += 10 * QVector3D::dotProduct(boid->vector_ver, position_diff) / position_diff.
                    lengthSquared();
            }
        }
    }
    return Sensor::on_overlay(obj, position_diff);
}
