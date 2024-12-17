//
// Created by root on 11/7/24.
//

#ifndef METAPROJECTILES_H
#define METAPROJECTILES_H
#include <qvectornd.h>
#include <string>
#include "MetaObject.h"
#include "MetaDrawable.h"

class MetaProjectiles :public MetaObject,public MetaDrawable{
public:
    float directDamage;
    float areaDamage;
    float areaRadius;
    int frame;
    QVector4D lightColor;
    float lightSize;
    std::vector<std::string > explode_effect;
    float life;
    float speed;
    bool instant =false;
    float deflectionPower;


};


#endif //METAPROJECTILES_H
