//
// Created by root on 11/18/24.
//

#include "MetaDrawable.h"

void MetaDrawable::init()
{
    if (image == "NONE")
    {
        texture_frames.push_back("NONE");
        return;
    }
    if (total_frames == 1)
    {
        texture_frames.push_back(image);
        return;
    }
    for (int i = 0; i < total_frames; ++i)
    {
        texture_frames.push_back(image + "_" + std::to_string(i));
    }
}
