# stainless warfare game design document
by 王德宇 2024013267 2024/12/19

## Introduction
stainless warfare is a 2d rts game that is set in futuristic worl,  
where the player is in control of a troop of vehicles and soldiers, 
to fight against the zombies.
this game has a lot of features, such as:

1. a lot of different units: tanks, soldiers, helicopters, police, common people, etc.
2. a lot of different maps: city, forest, desert, etc.  
you can even visit pokemon world. 原神world, etc.
3. highly friendly for modding: you can create your own units, maps, etc.



## Tech details

### render
game ui is rendered by Qt framework, and the game world is rendered by OpenGL.
### physics
units are seen as circles, and the collision detection is done by simple circle collision detection.  
to optimize the performance, we use spatial partitioning to reduce the number of collision detection.  
the forces are calculated by simple physics formulas, angular included.
### ai
complete melee ai ,pathfinding(astar for single unit, and flowfield for multiunits) ai, and shooting ai.
### unit system
units are configured by .ini files and .png, and loaded by a simple parser.  
the whole system has such parts:
1. core
2. turret
3. projectile
4. effect
5. sound
6. arm
### map system
I used traditional tilemap system, and the map is configured by .tmx files, and loaded by parser(tmxlite) .
By using tmx object layer, I implemented a trigger system, including:
* event:  
    1. unit detect
    2. time reach

* action
    1. dialog
    2. info
    3. camera move
    4. camera set
    5. unit add
    6. unit move
    7. unit remove

## Problems and solutions
1. OpenGL startup  
install Nvidia driver on linux.  
set wsl to use windows gpu.  
add link opengl32 glu32 to run on windows.  
2. choose space partitioning method  
I gave up quadtree and use grid partitioning, after reading articles on the internet,
my goal is reducing times of collision detection, and grid partitioning is enough for this game.
3. multiple units pathfinding crashing between units  
if far ,use flowfield, if near go to target that are precomputed to avoid crashing.
4. pixel cord to world cord  
remake the whole game to unify the cord system.
5. adjust ui to fit the game  
learnt qss
6. perfect pixel  
opengl choose the nearest pixel to draw.
7. transparent png  
add opengl alpha  blend function 
8. opengl performance  
turn to use vbo and vao
9. qt multimedia bug  
I need to copy QMultimedia.dll to the deploy-folder/plugin on windows. 
qt cannot make sound on my linux machine till now...
10. nan
use assert to check every suspicious float number.  
learn  msvs debug out to find the game run too fast (less than 1ms per frame) on my win machine, thus caused a zero division error.


## Reference
this game is inspired by the game "Rusted Warfare", most config files are compatible with it.  
the most art resources are from "Rusted Warfare" modding community.  
tmx parse: tmxlite  
compression: zlib zstd  
framework: Qt6 opengl  

