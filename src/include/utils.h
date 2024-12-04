//
// Created by root on 12/2/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <qvectornd.h>


class utils
{
public:
    static float sign(float value);
    static float dir_of(QVector3D vector);
    static float dir_diff(QVector3D vector, float angle);
    static void angle_ensure(float& angle);
    static void limit(float& value, float min, float max);
    static float limit(QVector3D& v, float min_length, float max_length);
    static void linear_limit_soft_r(float& value, float min, float max, float soft);
    static void linear_limit_soft_r(QVector3D& v, float min_length, float max_length, float soft);
    static void linear_limit_max_soft_r(QVector3D& v, float max_length, float soft);
    static float linear_limit_soft_v(float value, float min, float max, float soft);
    static QVector3D linear_limit_soft_v(const QVector3D& v, float min_length, float max_length, float soft);
    static QVector3D linear_limit_max_soft_v(const QVector3D& v, float max_length, float soft);
    static bool within(const QVector3D& v1, const QVector3D& v2, float range);
};


#endif //UTILS_H
