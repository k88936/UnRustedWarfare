//
// Created by root on 11/5/24.
//

#ifndef METAATTACHABLE_H
#define METAATTACHABLE_H
#include <QMatrix4x4>

#include "MetaObject.h"

class MetaAttachable;

struct attachSlot {
    // QMatrix4x4 transform;
    float x;
    float y;
    bool isFixed{};
    bool visible{};
    MetaAttachable *attached;
};

class MetaAttachable:MetaObject{
public:
    MetaObject *host;
    std::vector<attachSlot> attachSlots;
    void operate();
    void transformTo(QMatrix4x4);
    // void spin(float);

};



#endif //METAATTACHABLE_H
