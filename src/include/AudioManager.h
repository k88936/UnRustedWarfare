//
// Created by root on 12/6/24.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H


#include "structures/sound_event.h"


class Game;

class AudioManager {
public:
    explicit AudioManager(Game *game);
    float global_volume = 0.5;
    std::list<QSoundEffect*> pool;
    Game* game;
    void play(QVector3D listener_pos);
};



#endif //AUDIOMANAGER_H
