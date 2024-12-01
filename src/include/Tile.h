//
// Created by root on 11/25/24.
//

#ifndef TILE_H
#define TILE_H
#include <Drawable.h>


class Tile : public Drawable {
public:
    int index=0;
    // int x,y;
    Tile(int index, int x, int y, float z);
    void draw() override;

};



#endif //TILE_H
