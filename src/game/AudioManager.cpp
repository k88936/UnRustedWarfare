//
// Created by root on 12/6/24.
//

#include "AudioManager.h"

#include <QSoundEffect>

#include "UnitConfigs.h"

void AudioManager::play(QVector3D listener_pos)
{
    Game::sound_event_config_map["NONE"].clear();
    for (const auto& [id, sound_events] : Game::sound_event_config_map)
    {
        int cnt = 0;
        for (auto sound_event : sound_events)
        {
            float dis = (sound_event.position - listener_pos).lengthSquared();
            if (dis > 400)continue;
            if (cnt > 8)break;
            QSoundEffect* sound = pool.front();
            // qDebug() << sound->isPlaying();
            // qDebug() << sound->source();
            // qDebug() << sound->status();
            pool.pop_front();
            if (sound->isPlaying())
            {
                pool.push_back(sound);
                sound = new QSoundEffect();
            }
            sound->setSource(UnitConfigs::sounds.at(id));
            sound->setVolume(global_volume * 5 / (dis + 5.0f));
            sound->play();
            pool.push_back(sound);
            cnt++;
        }
    }
    Game::sound_event_config_map.clear();
}

void AudioManager::init()
{
    UnitConfigs::sounds["NONE"] = UnitConfigs::sounds["empty"];

    for (int i = 0; i < 32; ++i)
    {
        pool.push_back(new QSoundEffect());
    }
}
