//
// Created by root on 11/11/24.
//

#include "Attachable.h"

#include <QMatrix4x4>

#include "utils.h"

Attachable::Attachable()
{
}

void Attachable::updateSlots(QMatrix4x4 transform)
{
}

void Attachable::setRelativeRotation(const float rotation)
{
    this->relative_rotation = rotation;

    utils::angle_ensure(this->relative_rotation);
}

void Attachable::addRelativeRotation(const float rotation)
{
    this->relative_rotation += rotation;
    utils::angle_ensure(this->relative_rotation);
}
