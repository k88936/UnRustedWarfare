//
// Created by root on 11/6/24.
//
#include <QDirIterator>
#include <QDir>

#include <fstream>
#include <map>
#include <string>
#include "UnitConfigs.h"

#include <iostream>

#include "Game.h"
#include "MetaArm.h"
#include "Unit.h"
#include "RenderEngine.h"
#include "utils/pugixml.hpp"

std::unordered_map<std::string, MetaUnit*> UnitConfigs::meta_units;
std::unordered_map<std::string, MetaProjectiles*> UnitConfigs::meta_projectiles;
std::unordered_map<std::string, MetaEffect*> UnitConfigs::meta_effects;
std::unordered_map<std::string, MetaImage> UnitConfigs::images;
std::unordered_map<std::string, QUrl> UnitConfigs::sounds;


class IniParser
{
public:
    using Section = std::map<std::string, std::string>;
    using IniData = std::map<std::string, Section>;

    static IniData parse(const QString& filePath)
    {
        IniData data;
        QFile file(filePath);

        if (!file.open(QFile::ReadOnly))
        {
            throw std::runtime_error("Could not open file");
        }
        QTextStream in{&file};
        std::string line, currentSection;
        while (!in.atEnd())
        {
            line = in.readLine().toStdString();
            if (line.empty())
            {
                continue;
            }
            line = trim(line);
            if (line.empty() || line[0] == '#')
            {
                continue;
            }
            if (line[0] == '[' && line.back() == ']')
            {
                currentSection = line.substr(1, line.size() - 2);
            }
            else
            {
                auto delimiterPos = line.find(':');
                if (delimiterPos != std::string::npos)
                {
                    std::string key = trim(line.substr(0, delimiterPos));
                    std::string value = trim(line.substr(delimiterPos + 1));
                    data[currentSection][key] = value;
                }
            }
        }
        return data;
    }

private:
    static std::string trim(const std::string& str)
    {
        const auto whitespace = " \t\n\r\f\v";
        const size_t start = str.find_first_not_of(whitespace);
        const size_t end = str.find_last_not_of(whitespace);
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }
};

void parse_effects_list(int id, const std::string& content, std::vector<std::string>& ans)
{
    for (const auto& effect : utils::split(content, ','))
    {
        if (effect == "NONE")return;

        std::vector<std::string> word_and_num = utils::split(effect, '*');
        int n = 1;
        if (word_and_num.size() == 2)n = std::stoi(word_and_num[1]);
        if (effect.starts_with("CUSTOM:"))
        {
            for (int i = 0; i < n; ++i)
            {
                ans.push_back(std::to_string(id) + "___" + word_and_num[0]);
            }
        }
        else
        {
            for (int i = 0; i < n; ++i)
            {
                ans.push_back(word_and_num[0]);
            }
        }
    }
}


void UnitConfigs::load_ini(const QString& path)
{
    static int unit_id = 0;
    constexpr float designed_FPS = 50;
    const float total_scale = 0.33;
    const float scale_rw2sw = total_scale * 1.0 / 20;
    const float turnSpeed_rw2sw = designed_FPS;
    const float speed_rw2sw = 1.0f * scale_rw2sw * designed_FPS;
    const float time_rw2sw = 0.5f / designed_FPS;
    const float animate_delay_rw2sw = 1;
    const float turret_turn_speed_factor = 2.5;
    // int index = 0;
    auto* unit = new MetaUnit();
    std::unordered_map<std::string, MetaTurret*> turret_data;
    std::unordered_map<std::string, MetaArm*> arm_data;
    IniParser::IniData data = IniParser::parse(path);
#ifdef DEBUG
    assert(data.size()>0);
#endif

    unit_id++;
    for (const auto&
         [section_id, snd] : data)
    {
        if (section_id.starts_with("turret_"))
        {
            auto turret = new MetaTurret();
            turret_data[section_id.substr(7)] = turret;
        }
        else if (section_id.starts_with("projectile_"))
        {
            auto projectile = new MetaProjectiles();
            meta_projectiles[std::to_string(unit_id) + "___" + section_id.substr(11)] = projectile;
        }
        else if (section_id.starts_with("effect_"))
        {
            auto effect = new MetaEffect();
            meta_effects[std::to_string(unit_id) + "___CUSTOM:" + section_id.substr(7)] = effect;
        }
        else if (section_id.starts_with("arm_"))
        {
            auto arm = new MetaArm();
            arm_data[section_id.substr(4)] = arm;
        }
    }
    for (const auto& [section_id, content] : data)
    {
        if (section_id == "core")
        {
            for (const auto& [fst, snd] : content)
            {
                if (fst == "name")unit->name = snd;
                else if (fst == "displayText")unit->displayText = snd;
                else if (fst == "displayDescription")unit->display_description = snd;
                    // else if (fst == "price")unit->price = std::stoi(snd);
                else if (fst == "maxHp")unit->max_hp = std::stof(snd);
                else if (fst == "techLevel")unit->tech_level = std::stoi(snd);
                else if (fst == "buildSpeed")unit->build_speed = std::stof(snd);
                else if (fst == "energyMax")unit->energy_max = std::stof(snd);
                else if (fst == "energyRegen")unit->energy_regen = std::stof(snd);
                else if (fst == "mass")unit->mass = std::stof(snd);
                else if (fst == "radius")unit->radius = std::stof(snd) * scale_rw2sw;
                else if (fst == "displayRadius")unit->display_radius = std::stof(snd) * scale_rw2sw;
                else if (fst == "fogOfWarSightRange")unit->fog_of_war_sight_range = std::stof(snd); //NO FACTOR HERE!!
                else if (fst == "transportSlotsNeeded")unit->transport_slots_needed = std::stoi(snd);
                else if (fst == "tags")unit->tags = utils::split(snd, ',');
                else if (fst == "soundOnNewSelection")
                    unit->sound_on_new_selection = utils::without_extend(
                        utils::split(snd, ','));
                else if (fst == "soundOnMoveOrder")
                    unit->sound_on_move_order = utils::without_extend(
                        utils::split(snd, ','));
                else if (fst == "soundOnDeath")unit->sound_on_death = utils::without_extend(utils::split(snd, ','));

                else if (fst == "soundOnHit")unit->sound_on_hit = utils::without_extend(utils::split(snd, ','));
                else if (fst == "soundOnFire")unit->sound_on_fire = utils::without_extend(utils::split(snd, ','));
                else if (fst == "soundOnMove")unit->sound_on_move = utils::without_extend(utils::split(snd, ','));
                else if (fst == "effectOnDeath")parse_effects_list(unit_id, snd, unit->effect_on_death);
                else if (fst == "isBio")unit->is_bio = snd == "true";

                    //
                else if (fst == "maxTransportingUnits")unit->max_transporting_units = std::stoi(snd);
                else if (fst == "transportUnitsRequireTag")unit->transport_units_require_tag = utils::split(snd, ',');
                else if (fst == "exit_x")unit->exit_y = std::stof(snd) * scale_rw2sw;
                else if (fst == "exit_y")unit->exit_x = std::stof(snd) * scale_rw2sw;
                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
#ifdef DEBUG
            assert(unit->mass>0);
            assert(!unit->name.empty());
            // assert(unit->radius>0);
#endif
        }
        else if (section_id == "graphics")
        {
            for (const auto& [fst, snd] : content)
            {
                if (fst == "total_frames")
                {
                    unit->total_frames = std::stoi(snd);
                }
                else if (fst == "image")
                {
                    unit->image = snd;
                }
                else if (fst == "image_wreak")unit->image_wreak = snd;
                else if (fst == "image_turret")unit->image_turret = snd;
                else if (fst == "scale")unit->scale = std::stof(snd) * scale_rw2sw;
                else if (fst == "scaleImagesTo")unit->scale *= std::stof(snd) * scale_rw2sw;
                    //
                    // else if (fst=="shadowOffsetX")unit->shadowOffset.setX(std::stof(snd) * scale_rw2sw);
                    // else if (fst=="shadowOffsetY")unit->shadowOffset.setY(std::stof(snd) * scale_rw2sw);
                    //
                else if (fst == "scaleTurretImagesTo")unit->scaleTurret *= std::stof(snd) * scale_rw2sw;
                else if (fst == "lock_body_rotation_with_main_turret") unit->lock_body_rotation_with_main_turret = snd== "true";
                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
            images[unit->image].frames = unit->total_frames;
        }
        else if (section_id == "attack")
        {
            for (const auto& [fst, snd] : content)
            {
                if (fst == "canAttack")unit->can_attack = snd == "true";
                else if (fst == "canAttackFlyingUnits")unit->can_attack_flying_units = snd == "true";
                else if (fst == "canAttackLandUnits")unit->can_attack_land_units = snd == "true";
                else if (fst == "canAttackUnderwaterUnits")unit->can_attack_under_water_units = snd == "true";
                else if (fst == "shootDelay")unit->shoot_delay = std::stof(snd) * time_rw2sw;
                else if (fst == "maxAttackRange")unit->max_attack_range = std::stof(snd) * scale_rw2sw;
                else if (fst == "showRangeUIGuide")unit->show_range_ui_guide = snd == "true";
                else if (fst == "turretMultiTargeting")unit->turret_multi_targeting = snd == "true";
                else if (fst == "turretSize")unit->turret_size = std::stof(snd);
                else if (fst == "turretTurnSpeed")
                    unit->turret_turn_speed = std::stof(snd) * turnSpeed_rw2sw *
                        turret_turn_speed_factor;
                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
        }
        else if (section_id == "movement")
        {
            for (const auto& [fst, snd] : content)
            {
                if (fst == "movementType")
                {
                    //TODO
                    if (snd == "LAND")unit->movement = LAND;
                    else if (snd == "AIR")unit->movement = AIR;
                    else if (snd == "HOVER")unit->movement = HOVER;
                    else if (snd == "SUB")unit->movement = SUB;
                }
                else if (fst == "moveSpeed")unit->move_speed = std::stof(snd) * speed_rw2sw;
                else if (fst == "moveAccelerationSpeed")unit->move_acc = std::stof(snd) * speed_rw2sw / time_rw2sw;
                else if (fst == "moveDecelerationSpeed")unit->move_dec = std::stof(snd) * speed_rw2sw / time_rw2sw;
                else if (fst == "maxTurnSpeed")unit->max_turn_speed = std::stof(snd) * turnSpeed_rw2sw;
                else if (fst == "turnAcceleration")unit->turn_acc = std::stof(snd) * turnSpeed_rw2sw / time_rw2sw;
                else if (fst == "targetHeight")unit->target_height = std::stof(snd);
                else if (fst == "moveIgnoringBody")unit->moveIgnoringBody = snd == "true";
                else if (fst == "joinGroup")unit->join_group = snd == "true";

                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
        }
        else
        {
            if (section_id.starts_with("turret_"))
            {
                auto* turret = turret_data[section_id.substr(7)];
                if (unit->image_turret != "NONE")
                {
                    turret->image = unit->image_turret;
                }
                turret->turn_speed = unit->turret_turn_speed;
                turret->delay = unit->shoot_delay;
                turret->scale = unit->scaleTurret;
                turret->range = unit->max_attack_range;
                bool attachToUnit = true;
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "barrelX")turret->barrel_position.setY(std::stof(snd) * scale_rw2sw);
                    else if (fst == "barrelY")turret->barrel_position.setX(std::stof(snd) * scale_rw2sw);
                    else if (fst == "x")turret->slot_translation.setY(std::stof(snd) * scale_rw2sw);
                    else if (fst == "y")turret->slot_translation.setX(std::stof(snd) * scale_rw2sw);
                    else if (fst == "image")turret->image = snd;
                    else if (fst == "turnSpeed")
                        turret->turn_speed = std::stof(snd) * turnSpeed_rw2sw *
                            turret_turn_speed_factor;
                    else if (fst == "turnAcc")
                        turret->turn_acc = std::stof(snd) * turnSpeed_rw2sw *
                            turret_turn_speed_factor / time_rw2sw;
                    else if (fst == "shootSound")turret->shoot_sound = snd;
                    else if (fst == "shootSoundVolume")turret->shoot_sound_volume = std::stof(snd);
                    else if (fst == "recoilOffset")turret->recoil_offset = std::stof(snd) * scale_rw2sw;
                    else if (fst == "recoilOutTime")turret->recoil_out_time = std::stof(snd) * time_rw2sw;
                    else if (fst == "recoilReturnTime")turret->recoil_return_time = std::stof(snd) * time_rw2sw;
                    else if (fst == "range")turret->range = std::stof(snd) * scale_rw2sw;
                    else if (fst == "rangeMin")turret->range_min = std::stof(snd) * scale_rw2sw;
                    else if (fst == "invisible")
                    {
                        turret->invisible = snd == "true";
                    }
                    else if (fst == "delay")
                    {
                        turret->delay = std::stof(snd) * time_rw2sw;
                    }
                    else if (fst == "turnSpeedAcceleration")turret->turn_speed_acceleration = std::stof(snd);
                    else if (fst == "attachedTo")
                    {
                        turret->attached_to = snd;
                        attachToUnit = false;
                    }
                    else if (fst == "canShoot")turret->can_shoot = snd == "true";
                    else if (fst == "shoot_light")
                    {
                        turret->shoot_light = utils::parse_color(snd);
                        turret->shoot_light.setW(0.6);
                    }
                    else if (fst == "shoot_sound")turret->shoot_sound = utils::without_extend(snd);
                    else if (fst == "shoot_sound_vol")turret->shoot_sound_volume = std::stof(snd);
                    else if (fst == "limitingMinRange")turret->limiting_min_range = std::stof(snd) * scale_rw2sw;
                    else if (fst == "projectile")turret->projectile = std::to_string(unit_id) + "___" + snd;
                    else if (fst == "slave")
                    {
                        turret->slave = snd == "true";
                        turret->slot_isFixed = snd == "true";
                    }
                    else if (fst == "shoot_flame")parse_effects_list(unit_id, snd, turret->shoot_flame);
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                turret->slot_isFixed = false;
                turret->slot_inVisible = turret->invisible;
                // turret->barrelPosition*=turret->scale;
                if (attachToUnit)
                {
                    unit->attached_turret.push_back(turret);
                }
                else
                {
                    turret_data[turret->attached_to]->attached_turrets.push_back(turret);
                }
                if (turret->turn_speed == -1)
                {
                    turret->turn_speed = unit->turret_turn_speed;
                }
                if (turret->range == -1)
                {
                    turret->range = unit->max_attack_range;
                }
                turret->init();
            }
            else if (section_id.starts_with("projectile_"))
            {
                auto* projectile = meta_projectiles[std::to_string(unit_id) + "___" + section_id.substr(11)];
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "directDamage")projectile->directDamage = std::stof(snd);
                    else if (fst == "areaDamage")projectile->areaDamage = std::stof(snd);
                    else if (fst == "areaRadius")projectile->areaRadius = std::stof(snd) * scale_rw2sw;
                    else if (fst == "image")
                    {
                        projectile->image = snd;
                    }
                    else if (fst == "frame")projectile->frame = std::stoi(snd);
                    else if (fst == "lightSize")projectile->lightSize = std::stof(snd);
                    else if (fst == "lightColor")projectile->lightColor = utils::parse_color(snd);
                    else if (fst == "explodeEffect")
                    {
                        parse_effects_list(unit_id, snd, projectile->explode_effect);
                    }
                    else if (fst == "life")projectile->life = std::stof(snd) * time_rw2sw;
                    else if (fst == "speed")projectile->speed = std::stof(snd) * speed_rw2sw;
                    else if (fst == "drawSize")projectile->scale = std::stof(snd) * total_scale;
                    else if (fst == "deflectionPower")projectile->deflectionPower = std::stof(snd);
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                images[projectile->image].is_raw_size = true;
                projectile->init();
            }
            else if (section_id.starts_with("effect_"))
            {
                auto* effect = meta_effects[std::to_string(unit_id) + "___CUSTOM:" + section_id.substr(7)];
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "total_frames")effect->total_frames = std::stoi(snd);
                    else if (fst == "attachedToUnit")effect->attached_to_unit = snd == "true";
                    else if (fst == "image")
                    {
                        effect->image = snd;
                    }
                    else if (fst == "stripIndex")
                    {
                        effect->image = snd + ".png";
                    }
                    else if (fst == "color") effect->color = utils::parse_color(snd);
                    else if (fst == "animateFrameStart")effect->animate_frame_start = std::stoi(snd);
                    else if (fst == "animateFrameEnd")effect->animate_frame_end = std::stoi(snd);
                    else if (fst == "animateFrameSpeed")
                    {
                        effect->animate_frame_delay = animate_delay_rw2sw * time_rw2sw / std::stof(snd);
                    }
                    else if (fst == "life")effect->life = std::stof(snd) * time_rw2sw;
                    else if (fst == "scaleFrom")effect->scale_from = std::stof(snd) * total_scale;
                    else if (fst == "scaleTo")effect->scale_to = std::stof(snd) * total_scale;
                    else if (fst == "alpha")effect->alpha = std::stof(snd);
                    else if (fst == "spawnChance")effect->spawn_chance = std::stof(snd);
                    else if (fst == "xSpeedRelative")effect->x_speed_relative = std::stof(snd);
                    else if (fst == "ySpeedRelative")effect->y_speed_relative = std::stof(snd);
                    else if (fst == "xSpeedAbsolute")effect->x_speed_absolute = std::stof(snd);
                    else if (fst == "ySpeedAbsolute")effect->y_speed_absolute = std::stof(snd);
                    else if (fst == "xSpeedRelativeRandom")effect->x_speed_relative_random = std::stof(snd);
                    else if (fst == "ySpeedRelativeRandom")effect->y_speed_relative_random = std::stof(snd);
                    else if (fst == "hSpeed")effect->h_speed = std::stof(snd) * scale_rw2sw;
                    else if (fst == "drawUnderUnits")effect->draw_under_units = snd == "true";
                    else if (fst == "fadeInTime")effect->fade_in_time = std::stof(snd);
                    else if (fst == "fadeOut")effect->fade_out = snd == "true";
                    else if (fst == "physics")effect->physics = snd == "true";
                    else if (fst == "priority")
                    {
                        if (snd == "high")effect->priority = high;
                        else if (snd == "medium")effect->priority = medium;
                        else if (snd == "low")effect->priority = low;
                    }
                    else if (fst == "atmospheric")effect->atmospheric = snd == "true";
                    else if (fst == "delayedStartTimer")effect->delayed_start_timer = std::stof(snd);
                    else if (fst == "alsoPlaySound")
                    {
                        auto name_and_volume = utils::split(snd, ':');
                        effect->also_play_sound = utils::without_extend(name_and_volume.at(0));
                        if (name_and_volume.size() == 2)
                            effect->also_play_sound_volume = std::stof(name_and_volume.at(1));
                        else
                        {
                            effect->also_play_sound_volume = 1;
                        }
                    }
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                images[effect->image].is_raw_size = true;
                images[effect->image].frames = effect->total_frames;
                effect->init();
            }
            else if (section_id.starts_with("arm_"))
            {
                auto arm = arm_data[section_id.substr(4)];
                // arm->scale = 0.1f * scale_rw2sw;
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "x")arm->slot_translation.setY(std::stof(snd) * scale_rw2sw);
                    else if (fst == "y")arm->slot_translation.setX(std::stof(snd) * scale_rw2sw);
                    else if (fst == "spinRate")arm->spin_speed = std::stof(snd) * turnSpeed_rw2sw;
                    else if (fst == "image_end")arm->image = snd;
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                arm->init();
                unit->attached_arm.push_back(arm);
                images[arm->image].is_raw_size = true;
                images[arm->image].scale = 0.5;
            }
            else
            {
                qDebug() << "missed section:" << section_id;
            }
        }
    }

#ifdef DEBUG
    if (unit->name.empty())
    {
        qDebug() << path;
        throw std::runtime_error("null name");
    };
#endif
    meta_units[unit->name] = unit;
    unit->init();
}

void UnitConfigs::scan_dir(const QString& path)
{
    QDirIterator iter(path, QDir::NoDotAndDotDot | QDir::AllEntries, QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        iter.next();
        if (iter.fileInfo().isFile())
        {
            QString file_name = iter.fileInfo().fileName();
            if (file_name.endsWith(".ini"))
            {
                load_ini(iter.filePath());
            }
            else if (file_name.endsWith(".png") || file_name.endsWith(".gif"))
            {
                // qDebug() << iter.filePath();
                // images[file_name.toStdString()].image = std::move(img);
                images[(file_name.toStdString())].image = QImage(iter.filePath());
            }
            else if (file_name.endsWith(".wav"))
            {
                sounds[utils::without_extend(file_name.toStdString())] = QUrl::fromLocalFile(iter.filePath());
            }
            else if (file_name.endsWith(".ogg"))
            {
                // throw std::runtime_error("ogg not supported");
                qDebug() << "ogg not supported: " << iter.filePath();
            }
        }
    }
}

void UnitConfigs::init()
{
    scan_dir("../resources/units/");
    scan_dir("../resources/assets/sound/");
    scan_dir("../resources/assets/drawables/");
    MetaEffect::init_builtin_effects();
    init_units_map();
}

void UnitConfigs::init_units_map()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("../resources/maps/units_map.xml");

    if (!result)
    {
        std::cerr << "XML parsed with errors, attr value: [" << doc.child("node").attribute("attr").value() << "]\n";
        std::cerr << "Error description: " << result.description() << "\n";
        std::cerr << "Error offset: " << result.offset << " (error at [..." << (doc.child("node").attribute("attr").
            value() + result.offset) << "]\n\n";
        return;
    }

    // Access the root node
    pugi::xml_node root = doc.child("tileset");

    // Iterate over child nodes
    for (pugi::xml_node child : root.children())
    {
        auto properties = child.child("properties").children();
        for (auto xml_node : properties)
        {
            pugi::xml_attribute v = xml_node.attribute("value");
            pugi::xml_attribute m = xml_node.attribute("map");
            if (v)
            {
                if (m)
                    mapped_unit_name[v.value()] = m.value();
                else
                {
                    mapped_unit_name[v.value()] = v.value();
                }
            }
        }
    }
}

std::unordered_map<std::string, std::string> UnitConfigs::mapped_unit_name;
