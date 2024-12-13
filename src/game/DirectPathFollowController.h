//
// Created by root on 12/13/24.
//

#ifndef DIRECTPATHFOLLOWCONTROLLER_H
#define DIRECTPATHFOLLOWCONTROLLER_H
#include "Controller.h"


class DirectPathFollowController : public Controller
{
    DirectPathFollowController(Unit* unit, FlowField* flow_field)
    ;
    void before() override;
    void step() override;
    void after() override;
    QVector3D dir_expected;
    FlowField * flow_field;
    bool arrived = false;
    QVector3D speed_target;
    float angular_target;
    bool can_drive=true;
};


#endif //DIRECTPATHFOLLOWCONTROLLER_H
