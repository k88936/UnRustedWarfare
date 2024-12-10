//
// Created by root on 12/6/24.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include "structures/sound_event.h"


class Game;

class AudioManager
{
public:
    std::list<QSoundEffect*> pool;
    std::unordered_map<std::string, std::vector< SoundEvent>> sound_event_config_map;
    void play(QVector3D listener_pos);
    AudioManager();
    ~AudioManager();
};


#endif //AUDIOMANAGER_H
