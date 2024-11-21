//
// Created by root on 11/14/24.
//

#include "Sensor.h"

Sensor::Sensor(float radius): Object(radius,0,0) {

}

bool Sensor::onOverlay(Object *obj, QVector3D positionDiff) {
    return false;
}
