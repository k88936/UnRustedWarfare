// Created by root on 12/2/24.
//

#include "utils.h"

#include <QVector2D>
#include <numbers>
#include <random>

#include "Game.h"

QVector4D utils::parse_color(const std::string& colorStr)
{
    QColor color(QString::fromStdString(colorStr));
    return {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
}

std::vector<std::string> utils::split(const std::string& str, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

// Trim from the start (left)
std::string utils::ltrim(std::string s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }));
    return s;
}

// Trim from the end (right)
std::string utils::rtrim(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
    {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// Trim from both ends
std::string utils::trim(std::string s)
{
    return ltrim(rtrim(std::move(s)));
}

void utils::parse_item_list(const std::string& content, std::vector<std::string>& ans)
{
    for (std::vector<std::string> effects = split(content, ','); auto& str : effects)
    {
        str = utils::trim(str);
        if (str == "NONE")break;
        std::vector<std::string> word_and_num = split(str, '*');
        int n = 1;
        if (word_and_num.size() == 2)n = std::stoi(word_and_num[1]);
        for (int i = 0; i < n; ++i)
        {
            ans.push_back(word_and_num[0]);
        }
    }
}

void utils::parse_item_set(const std::string& content, std::set<std::string>& ans)
{
    for (std::vector<std::string> effects = split(content, ','); auto& str : effects)
    {
        str = utils::trim(str);
        if (str == "NONE")break;
        std::vector<std::string> word_and_num = split(str, '*');
        ans.insert(word_and_num[0]);
    }
}

utils::animater::animater(Game* game, const float duration, const float from, const float to):
    duration_(duration * 1000), to_(to),
    from_(from)
{
    // qDebug()<<duration_;
    start_ = game->start_time;
}

void utils::animater::reset()
{
    start_ = QTime::currentTime();
}

float utils::animater::get_value()
{
    if (float passed = start_.msecsTo(QTime::currentTime()); passed >= duration_)
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

void utils::limit_r(float& value, const float min, const float max)
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

float utils::limit_v(float value, float min, float max)
{
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
    return value;
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

float utils::soft_approach(float from, float to, float soft)
{
    return from + (to - from) * soft;
}

QVector3D utils::soft_approach(const QVector3D& from, const QVector3D& to, float soft)
{
    return from + (to - from) * soft;
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

QVector3D utils::add_offset_x(const QVector3D& v, const float offset)
{
    return QVector3D(v.x() + offset, v.y(), v.z());
}

QVector3D utils::add_offset_y(const QVector3D& v, const float offset)
{
    return QVector3D(v.x(), v.y() + offset, v.z());
}

QVector3D utils::add_offset_z(const QVector3D& v, const float offset)
{
    return QVector3D(v.x(), v.y(), v.z() + offset);
}

QVector3D utils::set_offset_x(const QVector3D& v, const float offset)
{
    return QVector3D(offset, v.y(), v.z());
}

QVector3D utils::set_offset_y(const QVector3D& v, const float offset)
{
    return QVector3D(v.x(), offset, v.z());
}

QVector3D utils::set_offset_z(const QVector3D& v, const float offset)
{
    return QVector3D(v.x(), v.y(), offset);
}

bool utils::random_bool(float chance)
{
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    return distribution(generator) < chance;
}

bool utils::freq_bool(float freq, float delta)
{
    return random_bool(freq * delta);
}

std::string utils::without_extend(const std::string& file_name)
{
    auto pos = file_name.find_last_of('.');
    if (pos == std::string::npos)
    {
        return file_name;
    }
    return file_name.substr(0, pos);
}

std::vector<std::string> utils::without_extend(const std::vector<std::string>& file_names)
{
    std::vector<std::string> ans;
    ans.reserve(file_names.size());
    for (const auto& file_name : file_names)
    {
        ans.push_back(without_extend(file_name));
    }
    return ans;
}
