//
// Created by root on 11/18/24.
//

#include "Effect.h"

#include "Game.h"


Effect::Effect(MetaEffect* meta, const QVector3D position, const float rotation,const QVector3D &linear_velocity_base): Object(1, 1, 1)
{
    this->meta = meta;
    this->position = position;
    if (meta->drawUnderUnits)
    {
        this->position.setZ(-1);
    }
    if(meta->xSpeedAbsolute!=0||meta->ySpeedAbsolute!=0)
    {
        this->linearVelocity = linear_velocity_base + QVector3D(meta->xSpeedAbsolute, meta->ySpeedAbsolute, 0);
    }
    else
    {
        this->linearVelocity = linear_velocity_base + QVector3D(meta->xSpeedRelativeRandom * (rand() % 100) / 100 + meta->xSpeedRelative, meta->ySpeedRelativeRandom * (rand() % 100) / 100 + meta->ySpeedRelative, 0);
    }
    this->alpha = meta->alpha;
    this->rotation = rotation;
    this->scale = meta->scaleFrom;
    this->animate_timer = 0;
    Object::before();
}

void Effect::draw()
{
    render_transform.setToIdentity();
    render_transform.translate(position);
    render_transform.rotate(rotation, 0, 0, 1);
    render_transform.scale(this->scale);
    Game::var_image_draw_config_map[this->meta->texture_frames.at(frame_id)].push_back(this);
}

void Effect::after()
{
    this->scale = meta->scaleFrom + (meta->scaleTo - meta->scaleFrom) * (has_life) / meta->life;
    Object::after();
    animate_timer += Game::deltaTime;
    if (meta->fadeOut && has_life > meta->life - meta->fadeInTime)
    {
        this->alpha = meta->alpha * (meta->life - has_life) / meta->fadeInTime;
        this->color.setZ(alpha);
    }
    else if (has_life < meta->fadeInTime)
    {
        this->alpha = meta->alpha * has_life / meta->fadeInTime;
        this->color.setZ(alpha);
    }

    if ((this->has_life += Game::deltaTime) >= meta->life)
    {
        marked_for_delete = true;
    };
    if (animate_timer > meta->animateFrameDelay)
    {
        animate_timer -= meta->animateFrameDelay;
        frame_id++;
        if (frame_id > meta->animateFrameEnd)
        {
            marked_for_delete = true;
            // frame_id=meta->animateFrameEnd;
        }
    }
    if(!marked_for_delete)draw();
}
