//
// Created by root on 12/6/24.
//

#include "AudioManager.h"

#include <QSoundEffect>

#include "UnitConfigs.h"

void AudioManager::play(QVector3D listener_pos)
{
    for (auto event_config_map : sound_event_config_map)
    {
        int cnt = 0;
        float v = 0;

        for (auto sound_event : event_config_map.second)
        {
            v += 1.0 / ((sound_event.position - listener_pos).lengthSquared() + 1.0f);
            cnt++;
        }
        QSoundEffect* sound = UnitConfigs::sounds.at(event_config_map.first);
        sound->setVolume(v / cnt);
        if (sound->isPlaying())continue;
        sound->play();
    }
}

void AudioManager::init()
{
    // for (auto& [id, sound_effect] : UnitConfigs::sounds)
    // {
    //     QObject::connect(sound_effect, &QSoundEffect::playingChanged, [=](const bool playing)
    //     {
    //         if (!playing)
    //         {
    //             sound_effect->play();
    //         }
    //     });
    // }
}
