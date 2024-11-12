//
// Created by root on 11/10/24.
//

#include "Turret.h"

#include <cmath>

#include "Game.h"


Turret::Turret(MetaTurret *meta) {
    this->meta=meta;
    this->textureId=meta->image;
    for (const auto & slot : meta->attached_turrets) {
        auto turret_slot=
            attachSlot(slot.translation, slot.isFixed, slot.inVisible, new Turret(slot.attached));
        turrets_attached.push_back(turret_slot);
    }
    rotationSpeed=0;
}

void Turret::updatePosition(QMatrix4x4 transform, float rotation_base) {
    this->rotation=relative_rotation+rotation_base;
        transform.rotate(this->relative_rotation,0,0,1);
    QMatrix4x4 translated_transform=transform;
    for (auto slot: this->turrets_attached) {
        QMatrix4x4 push_this=transform;
        push_this.translate(slot.translation.toVector3D());
        slot.attached->updatePosition(push_this, this->rotation);
        slot.attached->position=transform*slot.translation;
    }

}

float Turret::aim(QVector3D target) {
    QVector3D relative = target - position.toVector3D();
    float angle_target = std::atan2(relative.y(), relative.x());

    float angle_diff = angle_target - rotation;
    if(fabsf(angle_diff)<meta->turnSpeed*Game::deltaTime) {
        rotation=angle_target;
        rotationSpeed=0;
        return 0;
    }
    if(angle_target<0) {
        rotationSpeed=-meta->turnSpeed;
    }
    else {
        rotationSpeed=meta->turnSpeed;
    }

    return angle_diff;
}

bool Turret::shoot() {
    //TODO gen projectile and effect
    return true;
}

void Turret::draw() {
    Game::image_draw_config_map[this->textureId].push_back(this);
    for (const auto &slot: turrets_attached) {
        if(slot.inVisible) {
            continue;
        }
        slot.attached->draw();
    }
}
