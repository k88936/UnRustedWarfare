//
// Created by root on 12/13/24.
//

#ifndef DIRECTPATHFOLLOWCONTROLLER_H
#define DIRECTPATHFOLLOWCONTROLLER_H
#include "Controller.h"


class DirectPathFollowController : public Controller
{
public:
    DirectPathFollowController(Unit* unit, FlowField* flow_field)
    ;
    void before() override;
    void step() override;
    void after() override;
    QVector3D dir_expected;
    FlowField * flow_field=nullptr;
    bool arrived = false;
    QVector3D speed_target;
    float angular_target=0;
    bool can_drive=true;
    bool finish;
};


#endif //DIRECTPATHFOLLOWCONTROLLER_H
