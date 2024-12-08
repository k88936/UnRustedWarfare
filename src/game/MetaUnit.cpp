//
// Created by root on 11/5/24.
//

#include "MetaUnit.h"

void MetaUnit::init()
{
    MetaDrawable::init();


    if (is_bio)
    {
        sounds_on_move.push_back("footstep0.wav");
    }
    else
    {
        sounds_on_move.push_back("tank_moving0.wav");
        sounds_on_move.push_back("tank_moving1.wav");
        sounds_on_move.push_back("tank_moving2.wav");
        sounds_on_move.push_back("tank_moving3.wav");
    }
}
