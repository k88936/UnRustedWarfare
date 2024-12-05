//
// Created by root on 11/5/24.
//

#ifndef METAEFFECT_H
#define METAEFFECT_H
#include <qvectornd.h>
#include <string>
#include <vector>

#include "MetaDrawable.h"
#include "MetaObject.h"

enum Priority {
    high = 0,
    medium = 1,
    low = 2
};

class MetaEffect :public MetaObject ,public  MetaDrawable{
public:
    bool attached_to_unit;
    int animate_frame_start=0;
    int animate_frame_end=0;
    float animate_frame_delay=1;
    float life; //secends
    float scale_from=1;
    float scale_to=1;
    float x_speed_relative_random=0;
    float y_speed_relative_random=0;
    float x_speed_relative=0;
    float y_speed_relative=0;
    float x_speed_absolute=0;
    float y_speed_absolute=0;
    float spawn_chance=1;
    float fade_in_time=0;
    bool fade_out=false;
    bool physics=false;
    float alpha=1;
    float h_speed;
    bool draw_under_units=false;
    Priority priority;
    bool atmospheric;
    float delayed_start_timer;
    std::string also_play_sound;
    float also_play_sound_volume;
    QVector4D color={1,1,1,1};
};


#endif //METAEFFECT_H
