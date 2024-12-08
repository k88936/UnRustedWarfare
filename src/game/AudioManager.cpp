//
// Created by root on 12/6/24.
//

#include "AudioManager.h"

#include <QSoundEffect>

#include "UnitConfigs.h"

void AudioManager::play(QVector3D listener_pos)
{
    for (const auto& [id, sound_events] : sound_event_config_map)
    {
        for (auto sound_event : sound_events)
        {
            float dis = (sound_event.position - listener_pos).lengthSquared();
            QSoundEffect* sound = pool.front();
            qDebug() << sound->isPlaying();
            qDebug() << sound->source();
            qDebug() << sound->status();
            pool.pop_front();
            if (sound->isPlaying())
            {
                pool.push_back(sound);
                sound = new QSoundEffect();
            }
            sound->setSource(UnitConfigs::sounds.at(id));
            sound->setVolume(global_volume / (dis + 1.0f));
            sound->play();
            pool.push_back(sound);
        }
    }
    sound_event_config_map.clear();
}

void AudioManager::init()
{
    UnitConfigs::sounds["NONE"] = UnitConfigs::sounds["empty.wav"];

    for (int i = 0; i < 32; ++i)
    {
        pool.push_back(new QSoundEffect());
    }
}
