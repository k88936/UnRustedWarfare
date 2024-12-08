//
// Created by root on 12/8/24.
//

#ifndef SOUND_EVENT_H
#define SOUND_EVENT_H
#include <QSoundEffect>
#include <QVector3D>

struct SoundEvent
{
    QVector3D position;
    float volume = 1;
    QSoundEffect* assigned = nullptr;

    SoundEvent(const QVector3D& position, const float volume): position(position), volume(volume)
    {
    }
};
#endif //SOUND_EVENT_H
