//
// Created by root on 11/14/24.
//

#ifndef SPACEDEVIDEENGINE_H
#define SPACEDEVIDEENGINE_H
#include <vector>
#include "Grid.h"

class Unit;
class GridsManager;

class GridsManager {
public:
    GridsManager();
    int width=128;
    int height =128;
    std::pmr::vector<std::pmr::vector<Grid*>> grids;
    const float grid_size;
    void clearGrids() const;
    // grid* whichIn(float x,float y) const;
    // void whichAcross(float x, float y, float r, std::vector<grid*> &gridsAcross) const;
    //
    void updateObject(Object *object) const;

};



#endif //SPACEDEVIDEENGINE_H
