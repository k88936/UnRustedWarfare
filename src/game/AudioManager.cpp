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
        float cnt = 0;
        float v = 0;

        for (auto sound_event : sound_events)
        {
            v += 1.0 / ((sound_event.position - listener_pos).lengthSquared() + 1.0f);
            cnt++;
        }
        QSoundEffect* sound = UnitConfigs::sounds.at(id);
        sound->setVolume(v / cnt);
        if (sound->isPlaying())continue;
        sound->play();
    }
    sound_event_config_map.clear();
}

void AudioManager::init()
{
    auto empty = new QSoundEffect();
    // empty->setSource(QUrl::fromLocalFile("../sound/empty.wav"));
    UnitConfigs::sounds["NONE"] = empty;
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
