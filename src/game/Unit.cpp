//
// Created by root on 11/5/24.
//

#include "Unit.h"

#include "Game.h"
#include "Sensor.h"


Unit::Unit(MetaUnit *meta, const QVector3D position, const float rotation): Attachable(),Drawable(),Object(meta->radius,meta->mass,meta->mass) {
    this->meta = meta;
    this->position = position;
    this->rotation = rotation;
    this->scale=meta->scale;
    for (const auto &slot: meta->attached_turret) {
        auto turret =new Turret(slot);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets.push_back(turret);
    }
    auto* mar=new Sensor(meta->maxAttackRange);
    watchers.push_back(mar);
    this->restitution=0.1;
    linearDampingDir = 0.4;
     linearDampingVer = 0.8;
     angularDamping = 0.6;
}

void Unit::updateSlots(QMatrix4x4 transform) {

    transform.rotate(this->relative_rotation, 0, 0, 1);
    for (auto slot: this->turrets) {
        QMatrix4x4 push_this = transform;
        push_this.translate(slot->slot_translation);
        slot->position = transform.map(slot->slot_translation);
        slot->setRotation(slot->relative_rotation+rotation);
        slot->updateSlots(push_this);
    }
    for (const auto watcher : watchers) {
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

void Unit::attack(const QVector3D & target) {
    for (const auto turret : turrets) {
         turret->attack(target);
    }
}

void Unit::draw() {
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation,0,0,1);
    render_transform.scale(this->scale);
    Game::image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
    for (const auto &slot: turrets) {
        if (slot->slot_inVisible) {
            continue;
        }
        slot->draw();
    }
}

void Unit::after() {
    Object::after();
    if (!this->isAttached) {
        QMatrix4x4 transform;
        transform.translate(this->position);
        transform.rotate(this->rotation, 0, 0, 1);
        updateSlots(transform);
    }
    this->draw();
}


