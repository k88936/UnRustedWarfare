//
// Created by root on 11/12/24.
//

#include "Projectile.h"

#include "UnitConfigs.h"
#include "Game.h"

Projectile::Projectile(MetaProjectiles* meta, int team, const QVector3D position, const float rotation,
                       const QVector3D& linear_velocity_base
): Object(0.3f, 10, 1000), Drawable()
{
    this->meta = meta;
    this->position = position;
    this->team = team;
    this->rotation = rotation;
    this->scale = meta->scale;
    Object::before();
    this->linearVelocity = linear_velocity_base + vector_dir * meta->speed;
}

void Projectile::draw()
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    Game::var_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
}

void Projectile::after()
{
    if (this->has_lived >= meta->life)
    {
        marked_for_delete = true;
    }
    this->has_lived += Game::deltaTime;
    Object::after();
    // if(!marked_for_delete)
    draw();
}

bool Projectile::on_overlay(Object* obj, QVector3D positionDiff)
{
    if (obj->team == this->team)
    {
        return false;
    }

    if (auto unit = dynamic_cast<Unit*>(obj))
    {
        hit(unit);
        marked_for_delete = true;
        return true;
    }
    return false;
}

void Projectile::hit(Unit* unit) const
{
    for (const auto& explode_effect : meta->explode_effect)
    {
        auto meta_effect = UnitConfigs::meta_effects.at(explode_effect);
        Game::addEffect(new Effect(meta_effect, position, rotation, unit->linearVelocity));
    }
    // Game::addEffect()
}
