//
// Created by root on 11/6/24.
//
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

QVector4D parseColor(const std::string& colorStr)
{
    QColor color(QString::fromStdString(colorStr));
    return {color.redF(), color.greenF(), color.blueF(), 1.0f};
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
        if(str=="NONE")return;
        std::vector<std::string> word_and_num = split(str, '*');
        int n = 1;
        if (word_and_num.size() == 2)n = std::stoi(word_and_num[1]);
        for (int i = 0; i < n; ++i)
        {
            ans.push_back(word_and_num[0]);
        }
    }
}

void UnitConfigs::loadIni(const QString& path)
{
    const float scale_rw2sw = 1.0 / 20;
    const float turnSpeed_rw2sw = Game::FPS;
    const float speed_rw2sw = scale_rw2sw * Game::FPS;
    const float time_rw2sw = 1.0f / Game::FPS;
    const float animate_speed_rw2sw = 0.3;
    // int index = 0;
    auto* unit = new MetaUnit();
    std::unordered_map<std::string, MetaTurret*> turretData;
    IniParser::IniData data = IniParser::parse(path);
    for (const auto&
         [section_id, snd] : data)
    {
        if (section_id.starts_with("turret"))
        {
            auto* turret = new MetaTurret();
            turretData[section_id.substr(7)] = turret;
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
                else if (fst == "displayDescription")unit->displayDescription = snd;
                else if (fst == "price")unit->price = std::stoi(snd);
                else if (fst == "maxHp")unit->maxHp = std::stof(snd);
                else if (fst == "techLevel")unit->techLevel = std::stoi(snd);
                else if (fst == "buildSpeed")unit->buildSpeed = std::stof(snd);
                else if (fst == "energyMax")unit->energyMax = std::stof(snd);
                else if (fst == "energyRegen")unit->energyRegen = std::stof(snd);
                else if (fst == "mass")unit->mass = std::stof(snd);
                else if (fst == "radius")unit->radius = std::stof(snd) * scale_rw2sw;
                else if (fst == "displayRadius")unit->displayRadius = std::stof(snd) * scale_rw2sw;
                else if (fst == "fogOfWarSightRange")unit->fogOfWarSightRange = std::stof(snd) * scale_rw2sw;
                else if (fst == "transportSlotsNeeded")unit->transportSlotsNeeded = std::stoi(snd);
                else if (fst == "tags")unit->tags = split(snd, ',');
                else if (fst == "soundsOnNewSelection")unit->soundsOnNewSelection = split(snd, ',');
                else if (fst == "soundsOnMoveOrder")unit->soundsOnMoveOrder = split(snd, ',');
                else if (fst == "soundsOnDeath")unit->soundsOnDeath = split(snd, ',');
                else if (fst == "soundsOnHit")unit->soundsOnHit = split(snd, ',');
                else if (fst == "soundsOnFire")unit->soundsOnFire = split(snd, ',');
                else if (fst == "soundsOnMove")unit->soundsOnMove = split(snd, ',');

                    //
                else if (fst == "maxTransportingUnits")unit->maxTransportingUnits = std::stoi(snd);
                else if (fst == "transportUnitsRequireTag")unit->transportUnitsRequireTag = split(snd, ',');
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

                else if (fst == "scaleTurretImagesTo")unit->scaleTurret *= std::stof(snd) * scale_rw2sw;
                else
                {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
            images[unit->image].frames = unit->total_frames;
            unit->init_frames();
        }
        else if (section_id == "attack")
        {
            for (const auto& [fst, snd] : content)
            {
                if (fst == "canAttack")unit->canAttack = snd == "true";
                else if (fst == "canAttackFlyingUnits")unit->canAttackFlyingUnits = snd == "true";
                else if (fst == "canAttackLandUnits")unit->canAttackLandUnits = snd == "true";
                else if (fst == "canAttackUnderwaterUnits")unit->canAttackUnderWaterUnits = snd == "true";
                else if (fst == "shootDelay")unit->shootDelay = std::stof(snd) * time_rw2sw;
                else if (fst == "maxAttackRange")unit->maxAttackRange = std::stof(snd);
                else if (fst == "showRangeUIGuide")unit->showRangeUIGuide = snd == "true";
                else if (fst == "turretMultiTargeting")unit->turretMultiTargeting = snd == "true";
                else if (fst == "turretSize")unit->turretSize = std::stof(snd);
                else if (fst == "turretTurnSpeed")unit->turretTurnSpeed = std::stof(snd) * turnSpeed_rw2sw;
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
                    if (snd == "land")unit->movement = LAND;
                    else if (snd == "air")unit->movement = AIR;
                    else if (snd == "hover")unit->movement = HOVER;
                    else if (snd == "sub")unit->movement = SUB;
                }
                else if (fst == "moveSpeed")unit->moveSpeed = std::stof(snd) * speed_rw2sw;
                else if (fst == "moveAccelerationSpeed")unit->moveAcc = std::stof(snd) * speed_rw2sw;
                else if (fst == "moveDecelerationSpeed")unit->moveDec = std::stof(snd) * speed_rw2sw;
                else if (fst == "maxTurnSpeed")unit->maxTurnSpeed = std::stof(snd) * turnSpeed_rw2sw;
                else if (fst == "turnAcceleration")unit->turnAcc = std::stof(snd) * turnSpeed_rw2sw;
                else if (fst == "targetHeight")unit->targetHeight = std::stof(snd);
                else if (fst == "joinGroup")unit->joinGroup = snd == "true";

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
                auto* turret = turretData[section_id.substr(7)];
                turret->turnSpeed = unit->turretTurnSpeed;
                turret->delay = unit->shootDelay;
                turret->scale = unit->scaleTurret;
                bool attachToUnit = true;
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "barrelX")turret->barrelPosition.setY(std::stof(snd) * scale_rw2sw);
                    else if (fst == "barrelY")turret->barrelPosition.setX(std::stof(snd) * scale_rw2sw);
                    else if (fst == "x")turret->slot_translation.setY(std::stof(snd) * scale_rw2sw);
                    else if (fst == "y")turret->slot_translation.setX(std::stof(snd) * scale_rw2sw);
                    else if (fst == "image")turret->image = snd;
                    else if (fst == "turnSpeed")turret->turnSpeed = std::stof(snd) * turnSpeed_rw2sw;
                    else if (fst == "turnAcc")turret->turnAcc = std::stof(snd);
                    else if (fst == "shootSound")turret->shootSound = snd;
                    else if (fst == "shootSoundVolume")turret->shootSoundVolume = std::stof(snd);
                    else if (fst == "recoilOffset")turret->recoilOffset = std::stof(snd);
                    else if (fst == "recoilOutTime")turret->recoilOutTime = std::stof(snd);
                    else if (fst == "recoilReturnTime")turret->recoilReturnTime = std::stof(snd);
                    else if (fst == "range")turret->range = std::stof(snd) * scale_rw2sw;
                    else if (fst == "rangeMin")turret->rangeMin = std::stof(snd) * scale_rw2sw;
                    else if (fst == "invisible")
                    {
                        turret->invisible = snd == "true";
                    }
                    else if (fst == "delay")
                    {
                        turret->delay = std::stof(snd) * time_rw2sw;
                    }
                    else if (fst == "turnSpeedAcceleration")turret->turnSpeedAcceleration = std::stof(snd);
                    else if (fst == "attachedTo")
                    {
                        turret->attachedTo = snd;
                        attachToUnit = false;
                    }
                    else if (fst == "canShoot")turret->canShoot = snd == "true";
                    else if (fst == "limitingMinRange")turret->limitingMinRange = std::stof(snd) * scale_rw2sw;
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
                    turretData[turret->attachedTo]->attached_turrets.push_back(turret);
                }
                if (turret->turnSpeed == -1)
                {
                    turret->turnSpeed = unit->turretTurnSpeed;
                }
                turret->init_frames();
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
                    else if (fst == "lightColor")projectile->lightColor = parseColor(snd);
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
                images[projectile->image].rawSize = true;
                projectile->init_frames();
            }
            else if (section_id.starts_with("effect"))
            {
                auto* effect = meta_effects["CUSTOM:" + section_id.substr(7)];
                for (const auto& [fst, snd] : content)
                {
                    if (fst == "total_frames")effect->total_frames = std::stoi(snd);
                    else if (fst == "attachedToUnit")effect->attachedToUnit = snd == "true";
                    else if (fst == "image")
                    {
                        effect->image = snd;
                    }
                    else if (fst == "animateFrameStart")effect->animateFrameStart = std::stoi(snd);
                    else if (fst == "animateFrameEnd")effect->animateFrameEnd = std::stoi(snd);
                    else if (fst == "animateFrameSpeed")
                    {
                        effect->animateFrameDelay = animate_speed_rw2sw * time_rw2sw / std::stof(snd);
                    }
                    else if (fst == "life")effect->life = std::stof(snd) * time_rw2sw;
                    else if (fst == "scaleFrom")effect->scaleFrom = std::stof(snd);
                    else if (fst == "scaleTo")effect->scaleTo = std::stof(snd);
                    else if (fst == "alpha")effect->alpha = std::stof(snd);
                    else if (fst == "spawnChance")effect->spawnChance = std::stof(snd);
                    else if (fst == "xSpeedRelative")effect->xSpeedRelative = std::stof(snd);
                    else if (fst == "ySpeedRelative")effect->ySpeedRelative = std::stof(snd);
                    else if (fst == "xSpeedAbsolute")effect->xSpeedAbsolute = std::stof(snd);
                    else if (fst == "ySpeedAbsolute")effect->ySpeedAbsolute = std::stof(snd);
                    else if (fst == "xSpeedRelativeRandom")effect->xSpeedRelativeRandom = std::stof(snd);
                    else if (fst == "ySpeedRelativeRandom")effect->ySpeedRelativeRandom = std::stof(snd);
                    else if (fst == "hSpeed")effect->hSpeed = std::stof(snd);
                    else if (fst == "drawUnderUnits")effect->drawUnderUnits = snd == "true";
                    else if (fst == "fadeInTime")effect->fadeInTime = std::stof(snd);
                    else if (fst == "fadeOut")effect->fadeOut = snd == "true";
                    else if (fst == "priority")
                    {
                        if (snd == "high")effect->priority = high;
                        else if (snd == "medium")effect->priority = medium;
                        else if (snd == "low")effect->priority = low;
                    }
                    else if (fst == "atmospheric")effect->atmospheric = snd == "true";
                    else if (fst == "delayedStartTimer")effect->delayedStartTimer = std::stof(snd);
                    else if (fst == "alsoPlaySound")
                    {
                        auto name_and_volume = split(snd, ':');
                        effect->alsoPlaySound = name_and_volume[0];
                        effect->alsoPlaySoundVolume = std::stof(name_and_volume[1]);
                    }
                    else
                    {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                images[effect->image].rawSize = true;
                images[effect->image].frames = effect->total_frames;
                effect->init_frames();
            }
            else
            {
                qDebug() << "missed section:" << section_id;
            }
        }
    }
    meta_units[unit->name] = unit;
}

void UnitConfigs::init()
{
    QString path = "../M2A3/";
    QDir dir(path);
    dir.setFilter(QDir::Files);
    // IniParser parser;
    QStringList files = dir.entryList();
    for (const auto& file_name : files)
    {
        if (file_name.endsWith(".ini"))
        {
            loadIni(path + file_name);
        }
        else if (file_name.endsWith(".png"))
        {
            images[file_name.toStdString()].image = QImage(path + file_name);
        }
    }
}
