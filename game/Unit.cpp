//
// Created by root on 11/5/24.
//

#include "Unit.h"

#include "Game.h"


Unit::Unit(MetaUnit *meta, const QVector3D position, const float rotation): Attachable() {
    this->meta = meta;
    this->position = QVector4D(position, 1);
    this->relative_rotation = rotation;
    this->textureId = meta->image;
    for (const auto &slot: meta->attached_turret) {
        auto turret_slot =
                attachSlot(slot.translation, slot.isFixed, slot.inVisible, new Turret(slot.attached));
        turrets.push_back(turret_slot);
    }
}

void Unit::updatePosition(QMatrix4x4 transform, const float rotation_base) {
    this->rotation = relative_rotation + rotation_base;
    if (!this->isAttached) {
        transform.translate(this->position.toVector3D());
    }
    transform.rotate(this->relative_rotation, 0, 0, 1);
    for (auto slot: this->turrets) {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot.translation.toVector3D());
        slot.attached->updatePosition(push_this, this->rotation);
        slot.attached->position = transform * slot.translation;
    }
}

void Unit::attack(QVector3D target) {
    for (const auto &slot: turrets) {
        float diff = slot.attached->aim(target);
        if (diff == 0) {
            slot.attached->shoot();
        }
    }
}

void Unit::draw() {
    Game::image_draw_config_map[this->textureId].push_back(this);
    for (const auto &slot: turrets) {
        if (slot.inVisible) {
            continue;
        }
        slot.attached->draw();
    }
}


