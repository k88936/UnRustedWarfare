//
// Created by root on 11/16/24.
//

#ifndef RESOURCES_H
#define RESOURCES_H
#include <qimage.h>


struct MetaImage
{
    QImage image;
    float scale=1;
    bool is_raw_size = false;
    bool rawRot=false;
    int frames = 1;
};
//             land water  cliff sub ;
enum movementType {
        LAND=0b1000,
        AIR=0b1111,
        HOVER=0b1100,
        SUB=0b0101,
    };
#endif //RESOURCES_H
