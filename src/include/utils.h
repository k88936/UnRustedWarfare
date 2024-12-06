//
// Created by root on 12/2/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <qdatetime.h>
#include <qvectornd.h>


class utils
{
public:
    class animater
    {
        // float start_=0;
        QTime start_;
        float duration_;

        float to_;
        float from_;

    public:
        animater(float duration, float from, float to);
        void reset();
        float get_value();
    };

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
    static QVector3D generate_random_small_vector(float max_offset);
    static QVector3D add_offset_x(const QVector3D& v, float offset);
    static QVector3D add_offset_y(const QVector3D& v, float offset);
    static QVector3D add_offset_z(const QVector3D& v, float offset);
    static QVector3D set_offset_x(const QVector3D& v, float offset);
    static QVector3D set_offset_y(const QVector3D& v, float offset);
    static QVector3D set_offset_z(const QVector3D& v, float offset);
};


#endif //UTILS_H
