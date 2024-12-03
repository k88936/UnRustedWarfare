//
// Created by root on 11/5/24.
//

#include "Drawable.h"

#include <random>

#include "Game.h"
void Drawable::draw() {
}
QVector3D Drawable::generate_random_small_vector(float maxOffset) {
    static std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(-maxOffset, maxOffset);
    return QVector3D(distribution(generator), distribution(generator), distribution(generator));
}