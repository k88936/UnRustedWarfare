//
// Created by root on 11/5/24.
//

#include "Unit.h"

#include "Game.h"


Unit::Unit(MetaUnit *meta, const QVector3D position, const float rotation): Attachable(),Drawable() {
    this->meta = meta;
    this->position = position;
    this->relative_rotation = rotation;
    this->textureId = meta->image;
    for (const auto &slot: meta->attached_turret) {
        auto turret =new Turret(slot);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets.push_back(turret);
    }
    this->radius=meta->radius;
    auto* mar=new Object();
    mar->radius=meta->maxAttackRange;
    watchers.push_back(mar);
}

void Unit::updateSlots(QMatrix4x4 transform, const float rotation_base) {
    if(relative_rotation>=180) {
        relative_rotation-=360;
    }else if(relative_rotation<-180) {
        relative_rotation+=360;
    }
    this->rotation=relative_rotation+rotation_base;
    if(rotation>=180) {
        rotation-=360;
    }else if(rotation<-180) {
        rotation+=360;
    }

    if (!this->isAttached) {
        transform.translate(this->position);
    }
    transform.rotate(this->relative_rotation, 0, 0, 1);
    for (auto slot: this->turrets) {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot->slot_translation);
        slot->position = transform.map(slot->slot_translation);
        slot->updateSlots(push_this, this->rotation);
    }
    for (auto watcher : watchers) {
        watcher->position = this->position;
    }
}

// void Unit::attack(QVector3D target) {
//     for (const auto &slot: turrets) {
//         float diff = slot->aim(target);
//         if (diff == 0) {
//             slot->shoot();
//         }
//     }
// }

void Unit::draw() {
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation,0,0,1);
    render_transform.scale(this->scale);
    Game::image_draw_config_map[this->textureId].push_back(this);
    for (const auto &slot: turrets) {
        if (slot->slot_inVisible) {
            continue;
        }
        slot->draw();
    }
}


