//
// Created by root on 11/5/24.
//

#include "Object.h"

#include <MapConfig.h>


#include "Game.h"
#include "utils.h"

// Object::Object(QVector3D position, float rotation) {
//     this->position=position;
//     this->rotation=rotation;
// }


Object::Object(float radius, float mass, float inertia)
{
    this->mass = mass;
    this->inertia = inertia;
    this->radius = radius;
    this->invMass = 1 / mass;
    this->invInertia = 1 / inertia;
}

void Object::setRotation(const float rotation)
{
    this->rotation = rotation;
    while (this->rotation >= 180)
    {
        this->rotation -= 360;
    }
    while (this->rotation < -180)
    {
        this->rotation += 360;
    }
}

void Object::addRotation(const float rotation)
{
    this->rotation += rotation;
    while (this->rotation >= 180)
    {
        this->rotation -= 360;
    }
    while (this->rotation < -180)
    {
        this->rotation += 360;
    }
}

void Object::before()
{
    this->checked = false;
    this->linearForces *= 0;
    this->angularForces = 0;
    vector_dir = QVector3D(std::cos(rotation * M_PIf / 180), std::sin(rotation * M_PIf / 180), 0);
    vector_ver = QVector3D(-std::sin(rotation * M_PIf / 180), std::cos(rotation * M_PIf / 180), 0);
}

void Object::step()
{
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
                const bool thisAgree = this->onOverlay(tar, positionDiff);
                const bool targetAgree = tar->onOverlay(this, positionDiff);
                if (thisAgree && targetAgree)
                {
                    solveCollision(this, tar, positionDiff);
                }
            }
        }
    }
}


void Object::after()
{
    // linearVelocity*=(1-linearDamping);
    // angularVelocity*=(1-angularDamping);


    linearVelocity += (linearForces) * Game::deltaTime * invMass;

    float linearVelocityDir = QVector3D::dotProduct(linearVelocity, vector_dir);
    if (fabsf(linearVelocityDir) < linearDampingDir * Game::deltaTime)
    {
        linearVelocity -= vector_dir * linearVelocityDir;
        // float linearForcesDir=QVector3D::dotProduct(linearForces,Dir);
    }
    else
    {
        linearVelocity -= vector_dir * utils::sign(linearVelocityDir) * linearDampingDir * Game::deltaTime;
    }
    float linearVelocityVer = QVector3D::dotProduct(linearVelocity, vector_ver);
    if (fabsf(linearVelocityVer) < linearDampingVer * Game::deltaTime)
    {
        linearVelocity -= vector_ver * linearVelocityVer;
    }
    else
    {
        linearVelocity -= vector_ver * utils::sign(linearVelocityVer) * linearDampingVer * Game::deltaTime;
    }
    angularVelocity += (angularForces) * Game::deltaTime * invInertia;
    if (fabsf(angularVelocity) < angularDamping * Game::deltaTime)
    {
        angularVelocity = 0;
    }
    else
    {
        angularVelocity -= utils::sign(angularVelocity) * angularDamping * Game::deltaTime;
    }
    position += linearVelocity * Game::deltaTime;

    addRotation(angularVelocity * Game::deltaTime);


    // qDebug()<<"object::after";
    // qDebug()<<linearForces;
    // qDebug()<<angularForces;
    // qDebug()<<linearVelocity;
    // qDebug()<<angularVelocity;
    // qDebug()<<position;
    // qDebug()<<rotation;
    //
}

bool Object::onOverlay(Object* obj, QVector3D positionDiff)
{
    return true;
}

void Object::applyForce(QVector3D force, float torque)
{
    this->linearForces += force;
    this->angularForces += torque;
}

void Object::solveCollision(Object* obj1, Object* obj2, QVector3D positionDiff)
{
    // float factor=obj1->radius+obj2->radius-positionDiff.l
    const QVector3D normDiff = positionDiff.normalized();
    const float speedProjected = QVector3D::dotProduct(normDiff, obj1->linearVelocity - obj2->linearVelocity);
    const float force = (2 - obj1->restitution - obj2->restitution) * (speedProjected + 5.0f) * obj1->mass * obj2->mass
        * (obj1->radius + obj2->radius) * (obj1->radius + obj2->radius) / (
            (obj1->mass + obj2->mass) * (positionDiff.lengthSquared() + 5.0f) * Game::deltaTime);;
    const QVector3D force3D = normDiff * force;
    // if (force!=force) {
    //      qDebug()<<"nan";
    //      throw std::runtime_error("nan");
    //  }
    const float torqueBefore = 256 * force * obj1->friction * obj2->friction;
    // qDebug()<<torqueBefore;
    obj1->applyForce(-force3D, torqueBefore * obj1->radius);
    obj2->applyForce(force3D, -torqueBefore * obj2->radius);
    // qDebug()<<"hit";
}
