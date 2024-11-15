//
// Created by root on 11/5/24.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <QVector3D>

#include "Grid.h"

enum ObjectType {
    static_body,
    dynamic_body,
};

class Object {
public:
    Object(float radius ,float mass, float inertia);

    QVector3D position = QVector3D(0, 0, 0);
    float radius;
    float rotation = 0; //east is zero
    void setRotation(float rotation);

    void addRotation(float rotation);

    //
    std::vector<Grid *> gridsAcross;
    bool checked = false;



    QVector3D linearVelocity = QVector3D(0, 0, 0);
    QVector3D linearForces= QVector3D(0, 0, 0);
    float angularVelocity = 0;
    float angularForces = 0;
    // float angularAcceleration;
    float linearDampingDir = 0.1;
    float linearDampingVer = 0.3;
    float angularDamping = 0.3;
    QVector3D vectorDir ;
    QVector3D vectorVer;
    float mass ;
    float invMass ;
    float inertia ;
    float invInertia ;
    ObjectType type = dynamic_body;
    bool sleep = false;
    float friction = 0.1;
    float restitution = 0.1;

    virtual void before();

    virtual void step();

    virtual void after();

    virtual void onOverlay(Object *obj, QVector3D positionDiff);
    void applyForce(QVector3D force, float torque);


private:
    static void solveCollision(Object *obj1, Object *obj2, QVector3D positionDiff);
};
#endif //OBJECT_H
