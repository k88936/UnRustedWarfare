//
// Created by root on 11/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <unordered_map>
#include <vector>

#include "Drawable.h"


class Game {
public:
   static  std::unordered_map<std::string,std::vector<Drawable *>> image_draw_config_map
    ;
    static float deltaTime;

};



#endif //GAME_H
