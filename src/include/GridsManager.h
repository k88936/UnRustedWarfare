//
// Created by root on 11/14/24.
//

#ifndef SPACEDEVIDEENGINE_H
#define SPACEDEVIDEENGINE_H
#include <vector>

#include "Object.h"
#include "structures/grid.h"

class GridsManager {
public:
    GridsManager();
    int x_in_which(float x) const;
    int y_in_which(float y) const;
    int width=0;
    int height =0;
    std::vector<std::vector<Grid*>> grids;
    const int  grid_size;
    void clear_grids() const;
    void init();
    // grid* whichIn(float x,float y) const;
    // void whichAcross(float x, float y, float r, std::vector<grid*> &gridsAcross) const;
    //
    void update_object(Object *object) const;

};

#endif //SPACEDEVIDEENGINE_H