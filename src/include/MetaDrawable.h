//
// Created by root on 11/18/24.
//

#ifndef METADRAWABLE_H
#define METADRAWABLE_H
#include <string>
#include <vector>


class MetaDrawable
{
public:
    int total_frames = 1;
    std::string image="NONE";
    float scale = 1;

   virtual void init();
    std::vector<std::string> texture_frames;

};


#endif //METADRAWABLE_H
