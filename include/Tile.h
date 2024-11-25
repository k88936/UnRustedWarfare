//
// Created by root on 11/25/24.
//

#ifndef TILE_H
#define TILE_H
#include <Drawable.h>


class Tile : public Drawable {
public:
    Tile(int index, int x, int y);
    int index=0;
    int x,y;
    void draw() override;

};



#endif //TILE_H
