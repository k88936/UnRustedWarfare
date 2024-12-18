//
// Created by root on 11/5/24.
//

#include "Object.h"
#include <MapConfig.h>
#include "Game.h"
#include "utils.h"
#include <numbers>
// Object::Object(QVector3D position, float rotation) {
//     this->position=position;
//     this->rotation=rotation;
// }


Object::Object(Game* game, const float radius, const float mass, const float inertia): game(game), team(-1)
{

    this->mass = mass;
    this->inertia = inertia;
    this->radius = radius;
    this->inv_mass = 1 / mass;
    this->inv_inertia = 1 / inertia;
}

void Object::before()
{
    this->checked = false;
    this->linear_forces *= 0;
    this->angular_forces = 0;
    vector_dir = QVector3D(std::cos(rotation * std::numbers::pi / 180), std::sin(rotation * std::numbers::pi / 180), 0);
    vector_ver = QVector3D(-std::sin(rotation * std::numbers::pi / 180), std::cos(rotation * std::numbers::pi / 180),
                           0);
}

void Object::step()
{
    // in_sight=game->warfare_fog_manager.in_light(this);

    this->checked = true;
    for (auto grid : this->gridsAcross)
    {
        for (auto tar : grid->objects)
        {
            if (tar->checked)
            {
                continue;
            }
            QVector3D positionDiff = (tar->position - this->position);
            // float length = positionDiff.length();
            if (positionDiff.lengthSquared() < (this->radius + tar->radius) * (this->radius + tar->radius))
            {
                const bool thisAgree = this->on_overlay(tar, positionDiff);
                const bool targetAgree = tar->on_overlay(this, -positionDiff);
                if (thisAgree && targetAgree)
                {
                    solve_collision(this, tar, positionDiff);
                }
            }
        }
    }

#ifdef DEBUG
    assert(!std::isnan(linear_forces.x()));
    assert(!std::isnan(angular_forces));
    assert(!std::isnan(angular_velocity));
    assert(!std::isnan(rotation));
    assert(!std::isnan(linear_forces.y()));
    assert(!std::isnan(linear_velocity.x()));
    assert(!std::isnan(linear_velocity.y()));
#endif
}


void Object::after()
{
    // linearVelocity*=(1-linearDamping);
    // angularVelocity*=(1-angularDamping);
#ifdef DEBUG
    assert(!std::isnan(linear_forces.x()));
    assert(!std::isnan(linear_forces.y()));
    assert(!std::isnan(angular_forces));
#endif

    linear_velocity += (linear_forces) * game->delta_time * inv_mass;
#ifdef DEBUG
    assert(!std::isnan(linear_velocity.x()));
    assert(!std::isnan(linear_velocity.y()));
#endif

    float linearVelocityDir = QVector3D::dotProduct(linear_velocity, vector_dir);
    if (fabsf(linearVelocityDir) < linear_damping_dir * game->delta_time)
    {
        linear_velocity -= vector_dir * linearVelocityDir;
        // float linearForcesDir=QVector3D::dotProduct(linearForces,Dir);
    }
    else
    {
        linear_velocity -= vector_dir * utils::sign(linearVelocityDir) * linear_damping_dir * game->delta_time;
    }
    float linearVelocityVer = QVector3D::dotProduct(linear_velocity, vector_ver);
    if (fabsf(linearVelocityVer) < linear_damping_ver * game->delta_time)
    {
        linear_velocity -= vector_ver * linearVelocityVer;
    }
    else
    {
        linear_velocity -= vector_ver * utils::sign(linearVelocityVer) * linear_damping_ver * game->delta_time;
    }
    angular_velocity += (angular_forces) * game->delta_time * inv_inertia;
    if (fabsf(angular_velocity) < angular_damping * game->delta_time)
    {
        angular_velocity = 0;
    }
    else
    {
        angular_velocity -= utils::sign(angular_velocity) * angular_damping * game->delta_time;
    }
    position += linear_velocity * game->delta_time;
    rotation += angular_velocity * game->delta_time;
    utils::angle_ensure_r(rotation);
    // addRotation(angularVelocity * Game::deltaTime);


#ifdef DEBUG
    assert(!std::isnan(linear_forces.x()));
    assert(!std::isnan(angular_forces));
    assert(!std::isnan(angular_velocity));
    assert(!std::isnan(rotation));
    assert(!std::isnan(linear_forces.y()));
    assert(!std::isnan(linear_velocity.x()));
    assert(!std::isnan(linear_velocity.y()));
#endif

    // qDebug()<<"object::after";
    // qDebug()<<linearForces;
    // qDebug()<<angularForces;
    // qDebug()<<linearVelocity;
    // qDebug()<<angularVelocity;
    // qDebug()<<position;
    // qDebug()<<rotation;
    //
}

bool Object::on_overlay(Object* obj, const QVector3D position_diff)
{
    return true;
}

void Object::on_collision(const QVector3D& force, const float torque, Object* other)
{
    apply_force(force, torque);
}

void Object::apply_force(const QVector3D& force, const float torque)
{
#ifdef DEBUG
    assert(!std::isnan(force.x()));
    assert(!std::isnan(force.y()));
    assert(!std::isnan(torque));
#endif
    this->linear_forces += force;
    this->angular_forces += torque;
}

void Object::solve_collision(Object* obj1, Object* obj2, const QVector3D position_diff)
{
    // float factor=obj1->radius+obj2->radius-positionDiff.l
    const QVector3D normDiff = position_diff.normalized();
    const float speedProjected = QVector3D::dotProduct(normDiff, obj1->linear_velocity - obj2->linear_velocity);
    const float force = (2 - obj1->restitution - obj2->restitution) * (speedProjected + 5.0f) * obj1->mass * obj2->mass
        * (obj1->radius + obj2->radius) * (obj1->radius + obj2->radius) / (
            (obj1->mass + obj2->mass) * (position_diff.lengthSquared() + 5.0f) * obj1->game->delta_time);;
    const QVector3D force3D = normDiff * force;
    // if (force!=force) {
    //      qDebug()<<"nan";
    //      throw std::runtime_error("nan");
    //  }
    const float torqueBefore = 256 * force * obj1->friction * obj2->friction;
    // qDebug()<<torqueBefore;
#ifdef DEBUG
    assert(!std::isnan(force));
    assert(!std::isnan(torqueBefore));
#endif
    obj1->on_collision(-force3D, torqueBefore * obj1->radius, obj2);
    obj2->on_collision(force3D, -torqueBefore * obj2->radius, obj1);
    // qDebug()<<"hit";
}

bool Object::is_valid(Object*& obj)
{
    if (obj == nullptr)
    {
        return false;
    }
    if (obj->marked_for_delete)
    {
        obj = nullptr;
        return false;
    }
    return true;
}

