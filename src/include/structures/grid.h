//
// Created by root on 11/14/24.
//

#ifndef GRID_H
#define GRID_H


class Object;

struct Grid
{
    Grid(int x, int y): x(x), y(y)
    {
    }

    int x, y;
    //  std::vector<Unit*> units;
    //  std::vector<Projectile*> projectiles;
    std::vector<Object*> objects;
};
#endif //GRID_H
