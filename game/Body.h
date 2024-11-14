//
// Created by root on 11/14/24.
//

#ifndef BODY_H
#define BODY_H
#include <QVector3D>

#include "Object.h"

enum ObjectType {
    static_body,
    dynamic_body,
    bullet,
};

class Body:public  Object {
public:
    QVector3D linearVelocity;
    // QVector3D linearAcceleration;
    float angularVelocity;
    // float angularAcceleration;
    float linearDamping;
    float angularDamping;
    float mass;
    float inertia;
    ObjectType type;
    bool sleep;
    float friction;
    float restitution;

};



#endif //BODY_H
