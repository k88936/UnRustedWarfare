//
// Created by root on 12/13/24.
//

#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "Game.h"


class Unit;

class Controller
{
public:
    Unit* unit_under_control = nullptr;
    explicit Controller(Unit* unit);
    virtual ~Controller();
    virtual void before();
    virtual void step();
    virtual void after();
};


#endif //CONTROLLER_H
