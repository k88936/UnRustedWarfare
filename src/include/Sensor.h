//
// Created by root on 11/14/24.
//

#ifndef SENSOR_H
#define SENSOR_H
#include "Object.h"


class Sensor:public Object {
public:    Sensor(float radius);

    bool onOverlay(Object *obj, QVector3D positionDiff) override;
};



#endif //SENSOR_H
