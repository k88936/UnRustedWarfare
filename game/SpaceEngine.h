//
// Created by root on 11/14/24.
//

#ifndef SPACEDEVIDEENGINE_H
#define SPACEDEVIDEENGINE_H
#include <vector>
#include "Object.h"
#include "Grid.h"

class Unit;
class SpaceEngine;

class SpaceEngine {
public:
    SpaceEngine();
int    width=128;
    int height =128;
    std::pmr::vector<std::pmr::vector<Grid*>> grids;
    const int grid_size=8;
    void clearGrids() const;
    // grid* whichIn(float x,float y) const;
    // void whichAcross(float x, float y, float r, std::vector<grid*> &gridsAcross) const;
    //
    void updateObject(Object *object) const;
};



#endif //SPACEDEVIDEENGINE_H
