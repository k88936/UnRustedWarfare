//
// Created by root on 11/5/24.
//

#ifndef METAATTACHABLE_H
#define METAATTACHABLE_H
#include <QMatrix4x4>

#include "MetaObject.h"

class MetaAttachable;

// template <typename T>
// struct  attachSlot {
//     // QMatrix4x4 transform;
//     QVector4D translation;
//     bool isFixed;
//     bool inVisible;
//     T *attached;
//     attachSlot(float x,float y,bool isFixed,bool visible,T *attached):translation(x,y,0,1),isFixed(isFixed),inVisible(visible),attached(attached){};
//     attachSlot(QVector4D translation,bool isFixed,bool visible,T *attached):translation(translation),isFixed(isFixed),inVisible(visible),attached(attached){};
//     attachSlot(float x,float y,float z,bool isFixed,bool visible,T *attached):translation(x,y,z,1),isFixed(isFixed),inVisible(visible),attached(attached){};
// };

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
