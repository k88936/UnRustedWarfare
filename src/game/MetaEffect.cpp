//
// Created by root on 11/5/24.
//

#include "MetaEffect.h"

#include "UnitConfigs.h"

void add_effect(const std::string& name, MetaEffect* effect)
{
    UnitConfigs::images[effect->image].is_raw_size = true;
    UnitConfigs::images[effect->image].frames = effect->total_frames;
    UnitConfigs::meta_effects[name] = effect;
    effect->init();
}
void MetaEffect::init_builtin_effects()
{

    auto null_effect = new MetaEffect();
    add_effect("none", null_effect);

    auto smoke = new MetaEffect();
    smoke->image = "smoke.png";
    smoke->total_frames=1;
    smoke->life = 1;
    smoke->x_speed_relative_random = 0.5;
    smoke->y_speed_relative_random = 0.5;
    smoke->scale_from = 0.5;
    smoke->scale_to = 1;
    smoke->fade_out = true;
    add_effect("smoke", smoke);

    auto  small = new MetaEffect();
    small->image = "flame.png";
    small->total_frames=5;
    small->life = 0.2;
    small->animate_frame_end=4;
    small->animate_frame_delay=0.05;
    small->scale=0.5;
    add_effect("small", small);

    auto large= new MetaEffect();
    large->image = "flame_large.png";
    large->total_frames=4;
    large->life = 0.2;
    large->animate_frame_end=3;
    large->animate_frame_delay=0.05;
    add_effect("large", large);

      auto medium= new MetaEffect();
    medium->image = "flame_large.png";
    medium->total_frames=4;
    medium->life = 0.2;
    medium->animate_frame_end=3;
    medium->animate_frame_delay=0.05;
    medium->scale=0.8;
    add_effect("medium", medium);

    auto smallExplosion = new MetaEffect();
    smallExplosion->image = "explode_big.png";
    smallExplosion->total_frames=14;
    smallExplosion->life = 0.2;
    smallExplosion->animate_frame_end=13;
    smallExplosion->animate_frame_delay=0.05;
    smallExplosion->scale=0.5;
    add_effect("smallExplosion", smallExplosion);

    auto largeExplosion = new MetaEffect();
    largeExplosion->image = "explode_big2.png";
    largeExplosion->total_frames=14;
    largeExplosion->life = 0.2;
    largeExplosion->animate_frame_end=13;
    largeExplosion->animate_frame_delay=0.05;
    largeExplosion->scale=1;
    add_effect("largeExplosion", largeExplosion);

}
