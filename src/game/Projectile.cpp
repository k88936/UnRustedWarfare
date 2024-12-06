//
// Created by root on 11/12/24.
//

#include "Projectile.h"

#include "UnitConfigs.h"
#include "Game.h"
#include "utils.h"

Projectile::Projectile(MetaProjectiles* meta, const int team, const QVector3D position, const float rotation,
                       const QVector3D& linear_velocity_base
): Object(0.3f, 10, 1000), Drawable()
{
    this->meta = meta;
    this->position = position;
    this->team = team;
    this->rotation = rotation;
    this->scale = meta->scale;
    Object::before();
    this->linear_velocity = linear_velocity_base + vector_dir * meta->speed;
}

void Projectile::draw()
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    Game::var_transparent_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
}

void Projectile::before()
{
    Game::grids_manager.update_object(this);
    Object::before();
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
    if (marked_for_delete)
    {
        return false;
    }
    if (obj->team == this->team)
    {
        return false;
    }

    if (const auto unit = dynamic_cast<Unit*>(obj))
    {
        unit->hp -= meta->directDamage;
        hit_effect(unit);
        if (meta->areaDamage != 0 && meta->areaRadius != 0)
        {
            //aoe damage = - center_damage/r**2 +center_damage
            const float factor = -meta->areaDamage / (meta->areaRadius * meta->areaRadius);
            for (const auto grids = Game::grids_manager.scan(position, meta->areaRadius); const auto grids_across :
                 grids)
            {
                for (const auto object : grids_across->objects) //has bug actually
                {
                    if (const auto u = dynamic_cast<Unit*>(object))
                    {
                        if (u->team == this->team)
                            continue;
                        QVector3D offset = (u->position - this->position);
                        if (const auto distance = offset.lengthSquared(); distance < meta->areaRadius * meta->
                            areaRadius)
                        {
                            u->hp -= factor * distance + meta->areaDamage;
                            u->apply_force(offset / (distance + 1) * mass * meta->areaDamage, 0);
                        }
                    }
                }
            }
        }


        marked_for_delete = true;
        return true;
    }
    return false;
}

void Projectile::hit_effect(const Unit* unit) const
{
   float diff=0;
    for (const auto& explode_effect : meta->explode_effect)
    {
        diff+=0.01;
        const auto meta_effect = UnitConfigs::meta_effects.at(explode_effect);
        Game::addEffect(new Effect(meta_effect, QVector3D(position.x(),position.y(),position.z()+diff), rotation, unit->linear_velocity));
    }
    // Game::addEffect()
}
