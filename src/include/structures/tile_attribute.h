//
// Created by root on 11/30/24.
//

#ifndef TILE_ATTRIBUTE_H
#define TILE_ATTRIBUTE_H

struct TileAttribute
{
    explicit TileAttribute(const unsigned type=0b0000, const float pass_cost=1):type(type),pass_cost(pass_cost){}
    unsigned int type;
    float pass_cost;
};

#endif //TILE_ATTRIBUTE_H
