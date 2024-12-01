//
// Created by root on 11/30/24.
//

#ifndef METAIMAGE_H
#define METAIMAGE_H
#include <qimage.h>

struct MetaImage
{
    QImage image;
    float scale = 1;
    bool is_raw_size = false;
    bool rawRot = false;
    int frames = 1;
};
#endif //METAIMAGE_H
