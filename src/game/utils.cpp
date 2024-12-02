//
// Created by root on 12/2/24.
//

#include "utils.h"

#include <QVector2D>

float utils::sign(const float value)
{
    if (value > 0)
    {
        return 1;
    }
    else if (value < 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

float utils::dir_of(const QVector3D vector)
{
    return std::atan2(vector.y(), vector.x()) / std::numbers::pi * 180;
}
float utils::dir_diff(const QVector3D vector,float angle)
{
    const float target=dir_of(vector);
    float diff=target-angle;
    if(diff>=180) {
        diff-=360;
    }else if(diff<-180) {
        diff+=360;
    }
    return diff;
}

void utils::angle_ensure(float& angle)
{
    while (angle >= 180)
    {
        angle -= 360;
    }
    while (angle < -180)
    {
        angle += 360;
    }
}
