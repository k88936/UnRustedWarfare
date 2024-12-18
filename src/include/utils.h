//
// Created by root on 12/2/24.
//

#ifndef UTILS_H
#define UTILS_H
#include <qdatetime.h>
#include <qvectornd.h>
#include <random>
#include <set>


class Game;

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
        animater(Game* game, float duration, float from, float to);
        void reset();
        float get_value();
    };

    class framer
    {
    public:
        int start, end = 0;
        float cur = 0;
        const float step_l = 0;

        framer(const int start, const int end, const float step): start(start), end(end), step_l(step)
        {
            cur = start;
        }

        int get_value(float step);
    };

    const std::string EMPTY_STR = "NONE";
    extern float sign(float value);
    extern float dir_of(QVector3D vector);
    extern float dir_diff(QVector3D vector, float angle);
    extern void angle_ensure_r(float& angle);
    extern float angle_ensure_v(float angle);
    extern void limit_r(float& value, float min, float max);
    extern float limit_v(float value, float min, float max);
    extern float limit(QVector3D& v, float min_length, float max_length);
    extern void linear_limit_soft_r(float& value, float min, float max, float soft);
    extern void linear_limit_soft_r(QVector3D& v, float min_length, float max_length, float soft);
    extern void linear_limit_max_soft_r(float& v, float max_length, float soft);
    extern void linear_limit_max_soft_r(QVector3D& v, float max_length, float soft);
    extern float linear_limit_soft_v(float value, float min, float max, float soft);
    extern QVector3D linear_limit_soft_v(const QVector3D& v, float min_length, float max_length, float soft);
    extern QVector3D linear_limit_max_soft_v(const QVector3D& v, float max_length, float soft);
    extern float soft_approach(float from, float to, float soft);
    extern QVector3D soft_approach(const QVector3D& from, const QVector3D& to, float soft);
    extern bool within(const QVector3D& v1, const QVector3D& v2, float range);
    extern QVector3D generate_random_small_vector(float max_offset);
    extern float  generate_random_small_float(float inertia);
    extern QVector3D add_offset_x(const QVector3D& v, float offset);
    extern QVector3D add_offset_y(const QVector3D& v, float offset);
    extern QVector3D add_offset_z(const QVector3D& v, float offset);
    extern QVector3D set_offset_x(const QVector3D& v, float offset);
    extern QVector3D set_offset_y(const QVector3D& v, float offset);
    extern QVector3D set_offset_z(const QVector3D& v, float offset);

    extern std::string ltrim(std::string s);
    extern std::string rtrim(std::string s);
    extern std::string trim(std::string s);

    template <typename T>
    extern T random_element(const std::vector<T>& vec)
    {
#ifdef DEBUG
        assert(!vec.empty());
#endif
        static std::default_random_engine generator;
        std::uniform_int_distribution<std::size_t> distribution(0, vec.size() - 1);
        return vec.at(distribution(generator));;
    }

    template <typename T>
    extern T random_element(const std::vector<T>& vec, T default_on_empty)
    {
        if (vec.empty())
        {
            return default_on_empty;
        }
        static std::default_random_engine generator;
        std::uniform_int_distribution<std::size_t> distribution(0, vec.size() - 1);
        return vec.at(distribution(generator));;
    }

    extern bool random_bool(float chance);
    extern bool freq_bool(float freq, float delta);
    extern std::string without_extend(const std::string& file_name);
    extern std::vector<std::string> without_extend(const std::vector<std::string>& file_names);
    extern std::vector<std::string> split(const std::string& str, const char delimiter);
    extern QVector4D parse_color(const std::string& colorStr);
    extern void parse_item_list(const std::string& content, std::vector<std::string>& ans);
    extern void parse_item_set(const std::string& content, std::set<std::string>& ans);


    namespace team
    {
        inline bool is_enemy(int src, int target)
        {
#ifdef DEBUG
            assert(src!=5211324);
            assert(target!=5211324);
#endif
            if (target == -1 || src == -1)
            {
                return false;
            }
            if (target == -2 || src == -2)
            {
                return true;
            }

            return src % 2 != target % 2;
        }

        inline bool is_allied(int src, int target)
        {
#ifdef DEBUG
            assert(src!=5211324);
            assert(target!=5211324);
#endif
            if (target == -1 || src == -1)
            {
                return false;
            }
            if (target == -2 || src == -2)
            {
                return false;
            }
            return src % 2 == target % 2;
        }
    }

};
#endif //UTILS_H
