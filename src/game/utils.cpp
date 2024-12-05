//
// Created by root on 12/2/24.
//

#include "utils.h"

#include <QVector2D>
#include <numbers>
#include <random>

#include "Game.h"

utils::animater::animater(const float duration, const float from, const float to): duration_(duration * 1000), to_(to),
    from_(from)
{
    // qDebug()<<duration_;
    start_ = Game::start_time;
}

void utils::animater::reset()
{
    start_ = QTime::currentTime();
}

float utils::animater::get_value()
{
    float passed = start_.msecsTo(QTime::currentTime());
    if (passed >= duration_)
    {
        return to_;
    }
    else
    {
        return from_ + (to_ - from_) * passed / duration_;
    }
}

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

float utils::dir_diff(const QVector3D vector, float angle)
{
    const float target = dir_of(vector);
    float diff = target - angle;
    if (diff >= 180)
    {
        diff -= 360;
    }
    else if (diff < -180)
    {
        diff += 360;
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

void utils::limit(float& value, const float min, const float max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
}

float utils::limit(QVector3D& v, const float min_length, const float max_length)
{
    const float length = v.length();
    if (length < min_length)
    {
        v = v.normalized() * min_length;
    }
    else if (length > max_length)
    {
        v = v.normalized() * max_length;
    }
    return length;
}

void utils::linear_limit_soft_r(float& value, const float min, const float max, const float soft)
{
    if (value < min)
    {
        value = min + (value - min) * soft;
    }
    else if (value > max)
    {
        value = max + (value - max) * soft;
    }
}

void utils::linear_limit_soft_r(QVector3D& v, const float min_length, const float max_length, const float soft)
{
    const float length = v.lengthSquared();
    if (length < min_length * min_length)
    {
        v = v.normalized() * min_length + (v - v.normalized() * min_length) * soft;
    }
    else if (length > max_length * max_length)
    {
        v = v.normalized() * max_length + (v - v.normalized() * max_length) * soft;
    }
}

void utils::linear_limit_max_soft_r(QVector3D& v, const float max_length, const float soft)
{
    if (v.lengthSquared() > max_length * max_length)
    {
        v = v.normalized() * max_length + (v - v.normalized() * max_length) * soft;
    }
}

float utils::linear_limit_soft_v(const float value, const float min, const float max, const float soft)
{
    if (value < min)
    {
        return min + (value - min) * soft;
    }
    if (value > max)
    {
        return max + (value - max) * soft;
    }
    return value;
}

QVector3D utils::linear_limit_soft_v(const QVector3D& v, const float min_length, const float max_length,
                                     const float soft)
{
    const float length = v.length();
    if (length < min_length)
    {
        return v.normalized() * min_length + (v - v.normalized() * min_length) * soft;
    }
    else if (length > max_length)
    {
        return v.normalized() * max_length + (v - v.normalized() * max_length) * soft;
    }
    return v;
}

QVector3D utils::linear_limit_max_soft_v(const QVector3D& v, const float max_length, const float soft)
{
    if (v.length() > max_length)
    {
        return v.normalized() * max_length + (v - v.normalized() * max_length) * soft;
    }
    return v;
}

bool utils::within(const QVector3D& v1, const QVector3D& v2, const float range)
{
    return (v1 - v2).lengthSquared() < range * range;
}

QVector3D utils::generate_random_small_vector(const float max_offset)
{
    static std::default_random_engine generator;
    std::uniform_real_distribution distribution(-max_offset, max_offset);
    return QVector3D(distribution(generator), distribution(generator), 0);
}
