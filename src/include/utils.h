//
// Created by root on 12/2/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <qvectornd.h>


class utils {
public:
    static float sign(float value);
    static float dir_of(QVector3D vector);
    static float dir_diff(QVector3D vector, float angle);
    static void angle_ensure(float& angle);
    static void limit(float& value, float min, float max);
    static void limit_soft_r(float& value, float min, float max, float soft);
    static float limit_soft_v(float value, float min, float max, float soft);
    static bool within(const QVector3D& v1, const QVector3D& v2, float range);
};



#endif //UTILS_H
