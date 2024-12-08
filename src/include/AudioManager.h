//
// Created by root on 12/6/24.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <map>
#include <string>
#include <vector>

#include "structures/sound_event.h"


class AudioManager {
public:
    float global_volume = 0.5;
    std::map<std::string,std::vector<SoundEvent>> sound_event_config_map;

    void play(QVector3D listener_pos);

    void init();
};



#endif //AUDIOMANAGER_H
