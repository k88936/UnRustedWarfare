//
// Created by root on 12/14/24.
//

#ifndef METAARM_H
#define METAARM_H
#include "MetaAttachable.h"
#include "MetaDrawable.h"

class MetaArm : public MetaAttachable, public MetaDrawable
{
public:
    float spin_speed = 0;
};


#endif //METAARM_H
