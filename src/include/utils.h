//
// Created by root on 12/2/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <qdatetime.h>
#include <qvectornd.h>
#include <random>


namespace utils
{
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

    extern float sign(float value);
    extern float dir_of(QVector3D vector);
    extern float dir_diff(QVector3D vector, float angle);
    extern void angle_ensure(float& angle);
    extern void limit(float& value, float min, float max);
    extern float limit(QVector3D& v, float min_length, float max_length);
    extern void linear_limit_soft_r(float& value, float min, float max, float soft);
    extern void linear_limit_soft_r(QVector3D& v, float min_length, float max_length, float soft);
    extern void linear_limit_max_soft_r(QVector3D& v, float max_length, float soft);
    extern float linear_limit_soft_v(float value, float min, float max, float soft);
    extern QVector3D linear_limit_soft_v(const QVector3D& v, float min_length, float max_length, float soft);
    extern QVector3D linear_limit_max_soft_v(const QVector3D& v, float max_length, float soft);
    extern bool within(const QVector3D& v1, const QVector3D& v2, float range);
    extern QVector3D generate_random_small_vector(float max_offset);
    extern QVector3D add_offset_x(const QVector3D& v, float offset);
    extern QVector3D add_offset_y(const QVector3D& v, float offset);
    extern QVector3D add_offset_z(const QVector3D& v, float offset);
    extern QVector3D set_offset_x(const QVector3D& v, float offset);
    extern QVector3D set_offset_y(const QVector3D& v, float offset);
    extern QVector3D set_offset_z(const QVector3D& v, float offset);

    template <typename T>
    extern T random_element(const std::vector<T>& vec)
    {
        static std::default_random_engine generator;
        std::uniform_int_distribution<std::size_t> distribution(0, vec.size() - 1);
        return vec[distribution(generator)];
    }

};


#endif //UTILS_H
