//
// Created by root on 11/14/24.
//

#include "SpaceEngine.h"

void SpaceEngine::clearGrids() const {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            grids[i][j]->objects.clear();
        }
    }
}

SpaceEngine::SpaceEngine() {
    //fill
   grids.resize(width);
    for (int i = 0; i < width; ++i) {
        grids[i].resize(height);
        for (int j = 0; j < height; ++j) {
            grids[i][j] = new Grid();
        }
    }
}

// grid *SpaceEngine::whichIn(const float x, const float y) const {
//     return grids[static_cast<int>(x) / grid_size][static_cast<int>(y) / grid_size];
// }
//
// void SpaceEngine::whichAcross(const float x, const float y, const float r, std::vector<grid *> &gridsAcross) const {
//     gridsAcross.clear();
//
//     int x1 = static_cast<int>(x - r / grid_size);
//     const int x2 = static_cast<int>(x + r / grid_size);
//     int y1 = static_cast<int>(y - r / grid_size);
//     const int y2 = static_cast<int>(y + r / grid_size);
//     while (x1 <= x2) {
//         while (y1 <= y2) {
//             gridsAcross.push_back(grids[x1][y1]);
//             y1++;
//         }
//         x1++;
//     }
// }


void SpaceEngine::updateObject(Object *object) const {
    object->gridsAcross.clear();
        const int x = static_cast<int>(object->position.x());
        const int y = static_cast<int>(object->position.y());
        const int r = static_cast<int>(object->radius);

        int x1 = x - r / grid_size;
        const int x2 = x + r / grid_size;
        int y1 = y - r / grid_size;
        const int y2 = y + r / grid_size;
        while (x1 <= x2) {
            while (y1 <= y2) {
                grids[x1][y1]->objects.push_back(object);
                object->gridsAcross.push_back(grids[x1][y1]);
                y1++;
            }
            x1++;
        }
}

