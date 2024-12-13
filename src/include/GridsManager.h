//
// Created by root on 11/14/24.
//

#ifndef SPACEDEVIDEENGINE_H
#define SPACEDEVIDEENGINE_H
#include <QVector3D>
#include <set>
#include <vector>

#include "Trigger.h"
#include "structures/grid.h"

class Unit;
class Game;

class GridsManager {
public:
    GridsManager();
    ~GridsManager();
    int x_in_which(float x) const;
    int y_in_which(float y) const;
    std::set<Unit*> scan_units(QVector3D pos_lb, QVector3D pos_rt);
    int width=0;
    int height =0;
    std::vector<std::vector<Grid*>> grids;
    const int  grid_size;
    void clear_grids() const;
    void init(Game* game);
    // grid* whichIn(float x,float y) const;
    // void whichAcross(float x, float y, float r, std::vector<grid*> &gridsAcross) const;
    //
    void update_object(Object *object) const;
    std::vector<Grid*> scan_grids(const QVector3D& pos, float radius);
    std::set<Unit*> scan_units(float pox_x, float pos_y, float width, float height);
};

#endif //SPACEDEVIDEENGINE_H
