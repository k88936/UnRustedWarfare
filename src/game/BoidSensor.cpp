//
// Created by root on 12/2/24.
//

#include "BoidSensor.h"

#include "Game.h"
#include "utils.h"

void BoidSensor::before()
{
    nearby_count = 1;
    Sensor::before();
    angular_target = 0;
    speed_target = 0;
    nearby_even_rotation = 0;
    nearby_even_speed = QVector3D(0, 0, 0);
}

void BoidSensor::step()
{
    Sensor::step();
    const int x_floor = static_cast<int>(boid->position.x());
    const int y_floor = static_cast<int>(boid->position.y());
    const int x_ceil = x_floor + 1;
    const int y_ceil = y_floor + 1;

    QVector3D flow_field_expected(
        (Game::flow_field_for_test->get_vector(x_floor, y_floor).x() + Game::flow_field_for_test->
                                                                       get_vector(x_floor, y_ceil).x()) * (x_ceil - boid
            ->
            position.x())
        + (Game::flow_field_for_test->get_vector(x_ceil, y_floor).x() + Game::flow_field_for_test->
                                                                        get_vector(x_ceil, y_ceil).x()) * (boid->
            position.
            x() - x_floor),
        (Game::flow_field_for_test->get_vector(x_floor, y_floor).y() + Game::flow_field_for_test->
                                                                       get_vector(x_ceil, y_floor).y()) * (y_ceil - boid
            ->
            position.y())
        + (Game::flow_field_for_test->get_vector(x_floor, y_ceil).y() + Game::flow_field_for_test->
                                                                        get_vector(x_ceil, y_ceil).y()) * (boid->
            position.
            y() - y_floor),
        0
    );

    if (flow_field_expected.length() < 0.1)
    {
        return;
    }
    flow_field_expected.normalize();
    angular_target += utils::dir_of(flow_field_expected) * 0.8;

    speed_target += 0.2 * boid->meta->moveSpeed;
}

void BoidSensor::after()
{
    angular_target += 0.2 * nearby_even_rotation / nearby_count;
    utils::angle_ensure(angular_target);

    float speed_projected = QVector3D::dotProduct(boid->linearVelocity, boid->vector_dir);
    float angle_step = boid->meta->maxTurnSpeed * (speed_projected)
        / boid->meta->moveSpeed;
    Sensor::after();
    float diff = angular_target - boid->rotation;
    utils::angle_ensure(diff);
    if (std::fabsf(diff) < angle_step * Game::deltaTime)
    {
        // unit0->rotation = target;
        boid->angularVelocity *= 0.8;
    }
    else
    {
        const float acc = boid->meta->turnAcc * boid->inertia;
        boid->apply_force(
            boid->vector_ver * speed_projected * boid->mass * boid->angularVelocity * std::numbers::pi / 180,
            utils::sign(diff) * acc);
        utils::limit_soft_r(boid->angularVelocity, angle_step * Game::deltaTime, -angle_step * Game::deltaTime, 0.8);
    }


    speed_target += nearby_even_speed.length() / nearby_count * 0.2;
    if (speed_projected > 0)
    {
        if (speed_target > speed_projected)
        {
            boid->apply_force(
                boid->mass * boid->vector_dir * boid->meta->moveAcc,
                0);
        }
        else
        {
            boid->apply_force(
                -boid->mass * boid->vector_dir * boid->meta->moveDec,
                0);
        }
    }
    else if (speed_projected < 0)

    {
        if (speed_target > speed_projected)
        {
            boid->apply_force(
                boid->mass * boid->vector_dir * boid->meta->moveDec,
                0);
        }
        else
        {
            boid->apply_force(
                -boid->mass * boid->vector_dir * boid->meta->moveAcc,
                0);
        }
    }
    else
    {
        boid->apply_force(
            boid->mass * boid->vector_dir * boid->meta->moveAcc,
            0);
    }
}

bool BoidSensor::on_overlay(Object* obj, const QVector3D position_diff)
{
    if (Unit* unit = dynamic_cast<Unit*>(obj))
    {
        if (unit->meta->name == "laoda")
        {
            this->nearby_even_rotation += unit->rotation;
            this->nearby_even_speed += unit->linearVelocity;
            //position
            nearby_count++;
        }
    }


    return Sensor::on_overlay(obj, position_diff);
}
