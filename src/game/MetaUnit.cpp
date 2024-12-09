//
// Created by root on 11/5/24.
//

#include "MetaUnit.h"

void MetaUnit::init()
{
    MetaDrawable::init();


    if (is_bio)
    {
        // sound_on_move.emplace_back("footstep0.wav");
    }
    else
    {
        sound_on_move.emplace_back("tank_moving0");
        sound_on_move.emplace_back("tank_moving1");
        sound_on_move.emplace_back("tank_moving2");
        sound_on_move.emplace_back("tank_moving3");
    }
}
