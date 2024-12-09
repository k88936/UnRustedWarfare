//
// Created by root on 11/6/24.
//
#include <QDirIterator>
#include <QDir>

#include <fstream>
#include <map>
#include <string>
#include "UnitConfigs.h"

#include <qimage.h>

#include "Game.h"
#include "Unit.h"
#include "RenderEngine.h"

std::unordered_map<std::string, MetaUnit*> UnitConfigs::meta_units;
std::unordered_map<std::string, MetaProjectiles*> UnitConfigs::meta_projectiles;
std::unordered_map<std::string, MetaEffect*> UnitConfigs::meta_effects;
std::unordered_map<std::string, MetaImage> UnitConfigs::images;
std::unordered_map<std::string, QUrl> UnitConfigs::sounds;

std::vector<std::string> split(const std::string& str, const char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

QVector4D parse_color(const std::string& colorStr)
{
    QColor color(QString::fromStdString(colorStr));
    return {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
}

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


void UnitConfigs::parse_effects(const std::string& content, std::vector<std::string>& ans)
{
    for (std::vector<std::string> effects = split(content, ','); const auto& str : effects)
    {
        if (str == "NONE")return;
        std::vector<std::string> word_and_num = split(str, '*');
        int n = 1;
        if (word_and_num.size() == 2)n = std::stoi(word_and_num[1]);
        for (int i = 0; i < n; ++i)
        {
            ans.push_back(word_and_num[0]);
        }
    }
}

void UnitConfigs::load_ini(const QString& path)
{
    constexpr float designed_FPS=100;
    const float scale_rw2sw = 1.0 / 20;
    const float turnSpeed_rw2sw = designed_FPS;
    const float speed_rw2sw = 1.0f * scale_rw2sw * designed_FPS;
    const float time_rw2sw = 1.0f / designed_FPS;
    const float animate_delay_rw2sw = 1;
    const float turret_turn_speed_factor = 2.5;
    // int index = 0;
    auto* unit = new MetaUnit();
    std::unordered_map<std::string, MetaTurret*> turret_data;
    IniParser::IniData data = IniParser::parse(path);
    for (const auto&
         [section_id, snd] : data)
    {
        if (section_id.starts_with("turret"))
        {
            auto* turret = new MetaTurret();
            turret_data[section_id.substr(7)] = turret;
        }
        else if (section_id.starts_with("projectile"))
        {
            auto* projectile = new MetaProjectiles();
            meta_projectiles[section_id.substr(11)] = projectile;
        }
        else if (section_id.starts_with("effect"))
        {
            auto* effect = new MetaEffect();
            meta_effects["CUSTOM:" + section_id.substr(7)] = effect;
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
                else if (fst == "price")unit->price = std::stoi(snd);
                else if (fst == "maxHp")unit->max_hp = std::stof(snd);
                else if (fst == "techLevel")unit->tech_level = std::stoi(snd);
                else if (fst == "buildSpeed")unit->build_speed = std::stof(snd);
                else if (fst == "energyMax")unit->energy_max = std::stof(snd);
                else if (fst == "energyRegen")unit->energy_regen = std::stof(snd);
                else if (fst == "mass")unit->mass = std::stof(snd);
                else if (fst == "radius")unit->radius = std::stof(snd) * scale_rw2sw;
                else if (fst == "displayRadius")unit->display_radius = std::stof(snd) * scale_rw2sw;
                else if (fst == "fogOfWarSightRange")unit->fog_of_war_sight_range = std::stof(snd) * scale_rw2sw;
                else if (fst == "transportSlotsNeeded")unit->transport_slots_needed = std::stoi(snd);
                else if (fst == "tags")unit->tags = split(snd, ',');
                else if (fst == "soundOnNewSelection")unit->sound_on_new_selection = utils::without_extend(split(snd, ','));
                else if (fst == "soundOnMoveOrder")unit->sound_on_move_order = utils::without_extend(split(snd, ','));
                else if (fst == "soundOnDeath")unit->sound_on_death = utils::without_extend(split(snd, ','));
                else if (fst == "soundOnHit")unit->sound_on_hit = utils::without_extend(split(snd, ','));
                else if (fst == "soundOnFire")unit->sound_on_fire = utils::without_extend(split(snd, ','));
                else if (fst == "soundOnMove")unit->sound_on_move = utils::without_extend(split(snd, ','));
                else if (fst == "effectOnDeath")parse_effects(snd, unit->effect_on_death);
                else if (fst == "isBio")unit->is_bio = snd == "true";

                    //
                else if (fst == "maxTransportingUnits")unit->max_transporting_units = std::stoi(snd);
                else if (fst == "transportUnitsRequireTag")unit->transport_units_require_tag = split(snd, ',');
                else if (fst == "exit_x")unit->exit_y = std::stof(snd) * scale_rw2sw;
                else if (fst == "exit_y")unit->exit_x = std::stof(snd) * scale_rw2sw;
                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
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
                else if (fst == "scale")unit->scale = std::stof(snd) * scale_rw2sw;
                else if (fst == "scaleImagesTo")unit->scale *= std::stof(snd) * scale_rw2sw;
                    //
                    // else if (fst=="shadowOffsetX")unit->shadowOffset.setX(std::stof(snd) * scale_rw2sw);
                    // else if (fst=="shadowOffsetY")unit->shadowOffset.setY(std::stof(snd) * scale_rw2sw);
                    //
                else if (fst == "scaleTurretImagesTo")unit->scaleTurret *= std::stof(snd) * scale_rw2sw;
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
                else if (fst == "joinGroup")unit->join_group = snd == "true";

                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
        }
        else
        {
            if (section_id.starts_with("turret"))
            {
                auto* turret = turret_data[section_id.substr(7)];
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
                        turret->shoot_light = parse_color(snd);
                        turret->shoot_light.setW(0.6);
                    }
                    else if (fst == "shoot_sound")turret->shoot_sound = utils::without_extend(snd);
                    else if (fst == "shoot_sound_vol")turret->shoot_sound_volume = std::stof(snd);
                    else if (fst == "limitingMinRange")turret->limiting_min_range = std::stof(snd) * scale_rw2sw;
                    else if (fst == "projectile")turret->projectile = snd;
                    else if (fst == "slave")
                    {
                        turret->slave = snd == "true";
                        turret->slot_isFixed = snd == "true";
                    }
                    else if (fst == "shoot_flame")parse_effects(snd, turret->shoot_flame);
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
            else if (section_id.starts_with("projectile"))
            {
                auto* projectile = meta_projectiles[section_id.substr(11)];
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
                    else if (fst == "lightColor")projectile->lightColor = parse_color(snd);
                    else if (fst == "explodeEffect")
                    {
                        parse_effects(snd, projectile->explode_effect);
                    }
                    else if (fst == "life")projectile->life = std::stof(snd) * time_rw2sw;
                    else if (fst == "speed")projectile->speed = std::stof(snd) * speed_rw2sw;
                    else if (fst == "drawSize")projectile->scale = std::stof(snd);
                    else if (fst == "deflectionPower")projectile->deflectionPower = std::stof(snd);
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                images[projectile->image].is_raw_size = true;
                projectile->init();
            }
            else if (section_id.starts_with("effect"))
            {
                auto* effect = meta_effects["CUSTOM:" + section_id.substr(7)];
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
                    else if (fst == "color") effect->color = parse_color(snd);
                    else if (fst == "animateFrameStart")effect->animate_frame_start = std::stoi(snd);
                    else if (fst == "animateFrameEnd")effect->animate_frame_end = std::stoi(snd);
                    else if (fst == "animateFrameSpeed")
                    {
                        effect->animate_frame_delay = animate_delay_rw2sw * time_rw2sw / std::stof(snd);
                    }
                    else if (fst == "life")effect->life = std::stof(snd) * time_rw2sw;
                    else if (fst == "scaleFrom")effect->scale_from = std::stof(snd);
                    else if (fst == "scaleTo")effect->scale_to = std::stof(snd);
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
                        auto name_and_volume = split(snd, ':');
                        effect->also_play_sound = utils::without_extend(name_and_volume[0]);
                        effect->also_play_sound_volume = std::stof(name_and_volume[1]);
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
            else
            {
                qDebug() << "missed section:" << section_id;
            }
        }
    }

    meta_units[unit->name] = unit;
    unit->init();
}

void UnitConfigs::scan_dir(QString path)
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
                load_ini(path + file_name);
            }
            else if (file_name.endsWith(".png"))
            {
                qDebug() << iter.filePath();
                // images[file_name.toStdString()].image = std::move(img);
                images[(file_name.toStdString())].image = QImage(iter.filePath());
            }
            else if (file_name.endsWith(".wav"))
            {
                sounds[utils::without_extend(file_name.toStdString())] = QUrl::fromLocalFile(iter.filePath());
            }
            else if (file_name.endsWith(".ogg"))
            {
                throw std::runtime_error("ogg not supported");
            }
        }
    }
}

void UnitConfigs::init()
{
    scan_dir("../M2A3/");
    scan_dir("../sound/");
}
