//
// Created by root on 11/10/24.
//

#ifndef GAME_H
#define GAME_H
#include <QBasicTimer>
#include <unordered_map>
#include <vector>
#include "Structures.h"
#include "battlefieldwidget.h"
#include "Drawable.h"
#include "Effect.h"
#include "GridsManager.h"
#include "Projectile.h"
#include "Tile.h"



class Game{
public:
   static  std::unordered_map<std::string,std::vector<Drawable *>> var_image_draw_config_map;
   static std::unordered_map<std::string, std::vector<Drawable*>> const_image_draw_config_map;
   static float FPS;
   static float deltaTime;
   static GridsManager grids_manager;
   static std::vector<Unit*> units;
   static std::vector<Tile*> tiles;
   static QBasicTimer timer;
   static  void addProjectile(Projectile *projectile);
   static void addEffect(Effect* effect);
   static std::vector<Projectile*> projectiles;
   static std::vector<Effect*> effects;
   static BattlefieldWidget *battleFieldWidget;


   static void init();
   static void clean();
   static void step();

};



#endif //GAME_H
