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
};



#endif //UTILS_H
