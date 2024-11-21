//
// Created by root on 11/11/24.
//

#include "Attachable.h"

#include <QMatrix4x4>

Attachable::Attachable(){
}

void Attachable::updateSlots(QMatrix4x4 transform) {
}

void Attachable::setRelativeRotation(const float rotation) {
    this->relative_rotation=rotation;
    while(this->relative_rotation>=180) {
        this->relative_rotation-=360;
    }
    while(this->relative_rotation<-180) {
        this->relative_rotation+=360;
    }
}

void Attachable::addRelativeRotation(const float rotation) {
    this->relative_rotation+=rotation;
    while(this->relative_rotation>=180) {
        this->relative_rotation-=360;
    }
    while(this->relative_rotation<-180) {
        this->relative_rotation+=360;
    }
}



