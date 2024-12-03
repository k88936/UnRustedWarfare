//
// Created by root on 12/3/24.
//

#include "Flock.h"

Flock::Flock()
{

}

void Flock::move(const QVector3D & target)
{

    if (flow_field!=nullptr)
    {
        delete flow_field;
    }
        flow_field=new FlowField(target.x(), target.y(), movementType::LAND);
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
