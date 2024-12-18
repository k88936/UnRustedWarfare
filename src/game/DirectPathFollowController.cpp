//
// Created by root on 12/13/24.
//

#include "DirectPathFollowController.h"

#include "FlowField.h"
#include "Unit.h"

DirectPathFollowController::DirectPathFollowController(Unit* unit, FlowField* flow_field): Controller(unit),
    flow_field(flow_field)
{
}

void DirectPathFollowController::before()
{
}

void DirectPathFollowController::step()
{
    const int x_floor = static_cast<int>(unit_under_control->position.x());
    const int y_floor = static_cast<int>(unit_under_control->position.y());
    const int x_ceil = x_floor + 1;
    const int y_ceil = y_floor + 1;
    if (utils::within(unit_under_control->position, flow_field->end, unit_under_control->radius*2))
    {
        finish = true;
        // speed_target = (flow_field->end - unit_under_control->position) * unit_under_control->meta->move_dec /
        //     unit_under_control->meta->radius;
    }
    else
    {
        // flow_field_expected=flock->flow_field->get_vector(boid->position.x()+0.5, boid->position.y()+0.5);
        dir_expected = QVector3D(
            (flow_field->get_vector(x_floor, y_floor).x() + flow_field->get_vector(x_floor, y_ceil).x()) * (x_ceil-unit_under_control->position.x())
            + (flow_field->get_vector(x_ceil, y_floor).x() + flow_field->get_vector(x_ceil, y_ceil).x()) * (unit_under_control->position.x() - x_floor),
            (flow_field->get_vector(x_floor, y_floor).y() + flow_field->get_vector(x_ceil, y_floor).y()) * (y_ceil-unit_under_control->position.y())
            + (flow_field->get_vector(x_floor, y_ceil).y() + flow_field->get_vector(x_ceil, y_ceil).y()) * (unit_under_control->position.y() - y_floor),
            0
        );

        speed_target = dir_expected * unit_under_control->meta->move_speed;
    }
    angular_target = utils::dir_of(speed_target);
}

void DirectPathFollowController::after()
{
    if (finish)return;
    utils::angle_ensure_r(angular_target);
    float speed_projected = QVector3D::dotProduct(unit_under_control->linear_velocity, unit_under_control->vector_dir);
    float angle_step = unit_under_control->meta->max_turn_speed;

    if (unit_under_control->meta->is_wheel_powered)
    {
        angle_step *= (speed_projected) / unit_under_control->meta->move_speed;
    }
    float diff = angular_target - unit_under_control->rotation;
    utils::angle_ensure_r(diff);
    if (std::fabsf(diff) < angle_step * unit_under_control->game->delta_time)
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
            if (std::fabsf(diff) < angle_step * unit_under_control->game->delta_time * 4)
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
    }


    if (can_drive)
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
