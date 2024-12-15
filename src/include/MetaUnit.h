//
// Created by root on 11/5/24.
//

#ifndef METAUNIT_H
#define METAUNIT_H
#include <string>
#include <vector>

#include "MetaAttachable.h"
#include "MetaDrawable.h"
#include "MetaTurret.h"
#include "structures/movement_type.h"


class MetaArm;

class MetaUnit : public MetaAttachable, public MetaDrawable
{
public:
    //core
    std::string name; //name
    std::string displayText; //display name
    std::string display_description;
    int price;
    float max_hp;
    float mass;
    int tech_level;
    float radius; //contact radius
    float display_radius;
    float fog_of_war_sight_range = 10;
    int transport_slots_needed = 1;
    float build_speed;
    float energy_max;
    float energy_regen;
    std::vector<std::string> tags; //=std::vector<std::string>();
    //
    int max_transporting_units;
    std::vector<std::string> transport_units_require_tag; //=std::vector<std::string>();
    float exit_x;
    float exit_y;

    //graphics


    std::string image_wreak = "NONE";
    std::string image_turret = "NONE";
    // QVector3D shadowOffset=QVector3D(-0.3,-0.3,0);
    //sounds
    std::vector<std::string> sound_on_new_selection; //=std::vector<std::string>();
    std::vector<std::string> sound_on_move_order; //=std::vector<std::string>();
    std::vector<std::string> sound_on_death; //=std::vector<std::string>();
    std::vector<std::string> sound_on_hit; //=std::vector<std::string>();
    std::vector<std::string> sound_on_fire; //=std::vector<std::string>();
    std::vector<std::string> sound_on_move; //=std::vector<std::string>();
    //
    std::vector<std::string> effect_on_death; //=std::vector<std::string>();
bool lock_body_rotation_with_main_turret=false;

    //attack
    bool can_attack;
    bool can_attack_flying_units;
    bool can_attack_land_units;
    bool can_attack_under_water_units;
    float shoot_delay;
    float max_attack_range;
    bool show_range_ui_guide;
    bool turret_multi_targeting;
    float turret_size = 1;
    float turret_turn_speed;

    //movement
    movementType movement;
    float move_speed = 0;
    float move_acc = 1;
    float move_dec = 4;
    float max_turn_speed = 1;
    float turn_acc = 1;
    float target_height = 0;
    bool join_group;
    bool is_wheel_powered = false;

    bool moveIgnoringBody =false;

    std::vector<MetaTurret*> attached_turret; //=std::vector<MetaTurret*>();
    std::vector<MetaArm*> attached_arm; //=std::vector<MetaArm*>();

    float scaleTurret = 1;
    bool is_bio = false;

    void init();
};


#endif //METAUNIT_H
