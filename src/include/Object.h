//
// Created by root on 11/5/24.
//

#ifndef OBJECT_H
#define OBJECT_H
#include <QVector3D>
#include "structures/grid.h"


enum ObjectType
{
    static_body,
    dynamic_body,
};

class Object
{
public:
    virtual ~Object() = default;
    Object(float radius, float mass, float inertia);

    QVector3D position = QVector3D(0, 0, 0);
    float radius;
    float rotation = 0; //east is zero
    int team;
    //
    std::vector<Grid*> gridsAcross;
    bool checked = false;
    QVector3D linear_velocity = QVector3D(0, 0, 0);
    QVector3D linear_forces = QVector3D(0, 0, 0);
    float angular_velocity = 0;
    float angular_forces = 0;
    // float angularAcceleration;
    float linear_damping_dir = 0;
    float linear_damping_ver = 0;
    float angular_damping = 0;
    QVector3D vector_dir;
    QVector3D vector_ver;
    float mass;
    float inv_mass;
    float inertia;
    float inv_inertia;
    ObjectType type = dynamic_body;
    bool sleep = false;
    float friction = 0.1;
    float restitution = 0.1;

    int reference_count = 0;
    bool marked_for_delete = false;

    virtual void before();

    virtual void step();

    virtual void after();

    virtual bool on_overlay(Object* obj, QVector3D position_diff);
    void apply_force(QVector3D force, float torque);

    static bool is_valid(Object*& obj);
    static void ptr_change_to(Object*& obj, Object* new_obj);

private:
    static void solve_collision(Object* obj1, Object* obj2, QVector3D position_diff);
};
#endif //OBJECT_H
