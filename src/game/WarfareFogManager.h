//
// Created by root on 12/14/24.
//

#ifndef WARFAREFOGMANAGER_H
#define WARFAREFOGMANAGER_H
#include <QVector3D>
#include <vector>

#include "Drawable.h"
#include "Object.h"


class Game;

class WarfareFogManager
{
public:
    void init();

    explicit WarfareFogManager(Game* game): game(game)
    {
    };
    Game* game;

    std::vector<std::vector<float>> light_dense;


    void light(const Object* obj, float range);
    bool in_light(Object* obj);
    std::vector<Drawable> fogs;

    std::vector<std::string> fog_images;

    void draw();
    // inline int& dense_at(int x, int y)
    // {
    //     return light_dense.at(x + 1).at(y + 1);
    // }
};


#endif //WARFAREFOGMANAGER_H
