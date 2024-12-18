//
// Created by root on 12/3/24.
//

#include "Flock.h"

#include "BoidSensor.h"
#include "FlowField.h"
#include "utils.h"

Flock::Flock(Game* game): game(game)
{
}

void Flock::move(const QVector3D& target)
{
    pos_target = target;
    if (flow_field != nullptr)
    {
        delete flow_field;
    }
    QVector3D center;
    for (const auto boid : boids)
    {
        center += boid->position;
    }
    center /= boids.size();
    float radius_count = 0;
    for (auto unit : boids)
    {
        radius_count += unit->radius;
    }
    arrived_range = radius_count / sqrtf(boids.size());
    int movement = 0b1111;
    for (auto b : boids)
    {
        auto boid_sensor = new BoidSensor(b);
        if (b->controller != nullptr)
        {
            delete b->controller;
        }
        b->controller = boid_sensor;
        boid_sensor->target_offset = utils::linear_limit_max_soft_v(
            utils::linear_limit_max_soft_v(b->position - center, arrived_range * 4, 0.2), arrived_range * 2,
            0.5) + target;
        boid_sensor->arrived_flock_target = false;
        boid_sensor->arrived_flock_target_offset = false;
        boid_sensor->flock = this;
        movement &= b->meta->movement;
    }
    flow_field = new FlowField(game, target.x(), target.y(), movement);
}

void Flock::gather(const QVector3D& target)
{
    throw std::runtime_error("not implemented");
    pos_target = target;
    if (flow_field != nullptr)
    {
        delete flow_field;
    }
    flow_field = new FlowField(game, target.x(), target.y(), movementType::LAND);
    // for (const auto& boid : boids)
    // {
    //     center += boid->position;
    //     speed += boid->linearVelocity;
    // }
    // center /= boids.size();
    // speed /= boids.size();
    // for (const auto& boid : boids)
    // {
    //     boid->apply_force(boid->mass * (center - boid->position) * 0.1, 0);
    //     boid->apply_force(boid->mass * (speed - boid->linearVelocity) * 0.1, 0);
    // }
}
