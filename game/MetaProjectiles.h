//
// Created by root on 11/7/24.
//

#ifndef METAPROJECTILES_H
#define METAPROJECTILES_H
#include <qvectornd.h>
#include <string>


class MetaProjectiles {
public:
    float directDamage;
    float areaDamage;;
    float areaRadius;
    std::string image;
    int frame;
    QVector4D lightColor;
    float lightSize;
    std::string explodeEffect;
    float life;
    float speed;
    float drawSize;
    float deflectionPower;


};


#endif //METAPROJECTILES_H
