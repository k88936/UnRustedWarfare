//
// Created by root on 11/10/24.
//

#include "Turret.h"

#include <cmath>

#include "UnitConfigs.h"
#include "Effect.h"
#include "Game.h"
#include "utils.h"


Turret::Turret(MetaTurret *meta) : Attachable(), Drawable(), Sensor(meta->range) {
    this->meta = meta;
    for (const auto &slot: meta->attached_turrets) {
        auto turret = new Turret(slot);
        turret->slot_translation = slot->slot_translation;
        turret->slot_isFixed = slot->slot_isFixed;
        turret->slot_inVisible = slot->slot_inVisible;
        turrets_attached.push_back(turret);
    }
    this->scale=meta->scale;
}

void Turret::updateSlots(QMatrix4x4 transform) {
        transform.rotate(this->relative_rotation,0,0,1);
    for (auto slot: this->turrets_attached) {
        QMatrix4x4 push_this=transform;
        push_this.translate(slot->slot_translation);
        slot->position=transform.map(slot->slot_translation);
        slot->rotation=slot->relative_rotation+rotation;
        utils::angle_ensure(slot->rotation);
        slot->updateSlots(push_this);
    }

}

float Turret::aim(const QVector3D target) {
    QVector3D relative = target- position;

    float angle_target=utils::dir_of(relative);

    float angle_diff = angle_target - rotation;
    if(angle_diff>=180) {
        angle_diff-=360;
    }else if(angle_diff<-180) {
        angle_diff+=360;
    }
    if(fabsf(angle_diff)<meta->turnSpeed*Game::deltaTime) {
        relative_rotation+=angle_diff;
        rotationSpeed=0;
        return 0;
    }
    if(angle_diff<0) {
        rotationSpeed=-meta->turnSpeed;
    }
    else {
        rotationSpeed=+meta->turnSpeed;
    }
    relative_rotation+=rotationSpeed*Game::deltaTime;

    return angle_diff;
}

bool Turret::shoot() {

    if(!meta->canShoot) {
        return false;
    }
    if(coolDown<=0) {
        QMatrix4x4 transform;
        transform.translate(position);
        transform.rotate(rotation,0,0,1);
        const auto metaProjectiles=UnitConfigs::meta_projectiles.at(this->meta->projectile);
        Game::addProjectile(new Projectile(metaProjectiles, transform.map(meta->barrelPosition), rotation, QVector3D(0, 0, 0)));
        for (const auto & shoot_flame : meta->shoot_flame)
        {
            Game::addEffect(new Effect(UnitConfigs::meta_effects.at(shoot_flame), transform.map(meta->barrelPosition), rotation,this->linearVelocity));
        }
        coolDown=meta->delay;
    return true;
    }
    coolDown-=Game::deltaTime;
    return  false;
}


bool Turret::attack(const QVector3D &target) {
    const bool isAimed=aim(target)==0;
    for (const auto & attached : turrets_attached) {
        if(attached->meta->slave) {
            if(isAimed)attached->shoot();
        }else {
            attached->attack(target);
        }
    }
    // qDebug()<<meta->projectile;
    if(!isAimed) {
        return false;
    }
    return shoot();
}

void Turret::draw() {
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation,0,0,1);
    render_transform.scale(this->scale);
    Game::var_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
    for (const auto &slot: turrets_attached) {
        if(slot->slot_inVisible) {
            continue;
        }
        slot->draw();
    }
}
