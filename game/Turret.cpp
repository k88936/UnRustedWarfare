//
// Created by root on 11/10/24.
//

#include "Turret.h"

#include <cmath>

#include "Game.h"


Turret::Turret(MetaTurret *meta) : Attachable(), Drawable() {
    this->meta=meta;
    this->textureId=meta->image;
    for (const auto & slot : meta->attached_turrets) {
        auto turret=new Turret(slot);
        turret->slot_translation=slot->slot_translation;
        turret->slot_isFixed=slot->slot_isFixed;
        turret->slot_inVisible=slot->slot_inVisible;
        turrets_attached.push_back(turret);
    }
    rotationSpeed=0;
}

void Turret::updateSlots(QMatrix4x4 transform, float rotation_base) {

    if(relative_rotation>=180) {
        relative_rotation-=360;
    }else if(relative_rotation<-180) {
        relative_rotation+=360;
    }
    this->rotation=relative_rotation+rotation_base;
 if(rotation>=180) {
     rotation-=360;
 }
    else if (rotation<-180) {
        rotation+=360;
    }
        transform.rotate(this->relative_rotation,0,0,1);
    for (auto slot: this->turrets_attached) {
        QMatrix4x4 push_this=transform;
        push_this.translate(slot->slot_translation);
        slot->position=transform.map(slot->slot_translation);
        slot->updateSlots(push_this, this->rotation);
    }

}

float Turret::aim(const QVector3D target) {
    QVector3D relative = target- position;
    float angle_target = std::atan2(relative.y(), relative.x())/std::numbers::pi *180;


    float angle_diff = angle_target - rotation;
    if(angle_diff>=180) {
        angle_diff-=360;
    }else if(angle_diff<-180) {
        angle_diff+=360;
    }
    if(fabsf(angle_diff)<meta->turnSpeed*Game::deltaTime*10) {
        relative_rotation+=angle_diff;
        rotationSpeed=0;
        return 0;
    }else if(angle_diff<0) {
        rotationSpeed=-meta->turnSpeed;
    }
    else {
        rotationSpeed=+meta->turnSpeed;
    }
    relative_rotation+=rotationSpeed*Game::deltaTime*10;

    return angle_diff;
}

bool Turret::shoot() {
    //TODO gen projectile and effect
    return true;
}

void Turret::draw() {

    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation,0,0,1);
    render_transform.scale(this->scale);
    Game::image_draw_config_map[this->textureId].push_back(this);
    for (const auto &slot: turrets_attached) {
        if(slot->slot_inVisible) {
            continue;
        }
        slot->draw();
    }
}
