//
// Created by root on 11/5/24.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <QVector3D>

#include "structures/grid.h"


enum ObjectType {
    static_body,
    dynamic_body,
};

class Object {
public:
    virtual ~Object() = default;
    Object(float radius ,float mass, float inertia);

    QVector3D position = QVector3D(0, 0, 0);
    float radius;
    float rotation = 0; //east is zero
    int team;

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
    float linearDampingDir = 0;
    float linearDampingVer = 0;
    float angularDamping = 0;
    QVector3D vector_dir ;
    QVector3D vector_ver;
    float mass ;
    float invMass ;
    float inertia ;
    float invInertia ;
    ObjectType type = dynamic_body;
    bool sleep = false;
    float friction = 0.1;
    float restitution=0.1 ;

    bool marked_for_delete = false;

    virtual void before();

    virtual void step();

    virtual void after();

    virtual bool onOverlay(Object *obj, QVector3D positionDiff);
    void applyForce(QVector3D force, float torque);


private:
    static void solveCollision(Object *obj1, Object *obj2, QVector3D positionDiff);
};
#endif //OBJECT_H