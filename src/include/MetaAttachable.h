//
// Created by root on 11/5/24.
//

#ifndef METAATTACHABLE_H
#define METAATTACHABLE_H
#include <QMatrix4x4>

#include "MetaObject.h"

class MetaAttachable;

class MetaAttachable{
public:
    virtual ~MetaAttachable() = default;
    MetaAttachable();
    QVector3D slot_translation=QVector3D(0,0,0.1);
    bool slot_isFixed=false;
    bool slot_inVisible=false;
    // MetaObject *host;
    // std::vector<attachSlot> attachSlots;
    // void operate();
    // void transformTo(QMatrix4x4);
    // void spin(float);

};



#endif //METAATTACHABLE_H
