//
// Created by root on 11/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <QBasicTimer>
#include <QObject>
#include <unordered_map>
#include <vector>

#include "Drawable.h"
#include "GridsManager.h"
#include "Projectile.h"
#include "../mainwidget.h"

class Game{
public:
   static  std::unordered_map<std::string,std::vector<Drawable *>> image_draw_config_map;
   static float deltaTime;
   static GridsManager grids_manager;
   static std::vector<Unit*> units;
   static QBasicTimer timer;
   static std::vector<Projectile*> projectiles;
   static MainWidget *battleFieldWidget;


   static void init();
   static void step();

};



#endif //GAME_H
