//
// Created by root on 11/12/24.
//

#include "Projectile.h"

#include "Effect.h"
#include "UnitConfigs.h"
#include "Game.h"
#include "utils.h"

Projectile::Projectile(Game* game, MetaProjectiles* meta, const int team, const QVector3D position,
                       const float rotation,
                       const QVector3D& linear_velocity_base
): Object(game, 0.3f, 10, 1000), Drawable()
{
    this->meta = meta;
    this->position = position;
    this->team = team;
    this->rotation = rotation;
    this->scale = meta->scale;
    Object::before();
    this->linear_velocity = linear_velocity_base + vector_dir * meta->speed;
}

void Projectile::draw(Game* game)
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    game->var_transparent_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
}

void Projectile::before()
{
    game->grids_manager.update_object(this);
    Object::before();
}

void Projectile::after()
{
    if (this->has_lived >= meta->life)
    {
        marked_for_delete = true;
    }
    this->has_lived += game->deltaTime;
    Object::after();
    // if(!marked_for_delete)
    draw(game);
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
            for (const auto grids = game->grids_manager.scan(position, meta->areaRadius); const auto grids_across :
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
    for (const auto& explode_effect : meta->explode_effect)
    {
        const auto meta_effect = UnitConfigs::meta_effects.at(explode_effect);
        game->addEffect(new Effect(game, meta_effect, utils::set_offset_z(position, Game::LayerConfig::SHELL), rotation,
                                   this->linear_velocity));
    }
    // Game::addEffect()
}
