//
// Created by root on 11/16/24.
//

#ifndef RESOURCES_H
#define RESOURCES_H
#include <qimage.h>


struct MetaImage
{
    QImage image;
    float size = 1;
    bool rawSize = false;
    bool rawRot=false;
    int frames = 1;
};
#endif //RESOURCES_H
