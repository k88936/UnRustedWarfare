//
// Created by root on 11/6/24.
//

#include <fstream>
#include <map>
#include <QColor>
#include <string>
#include "Configs.h"

std::unordered_map<std::string, MetaUnit *> Configs::meta_units;
std::unordered_map<std::string, MetaTurret *> Configs::meta_turrets;
std::unordered_map<std::string, MetaProjectiles *> Configs::meta_projectiles;
std::unordered_map<std::string, MetaEffect *> Configs::meta_effects;


std::vector<std::string> split(const std::string &str, const char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

QVector4D parseColor(const std::string &colorStr) {
    QColor color(QString::fromStdString(colorStr));
    return QVector4D(color.redF(), color.greenF(), color.blueF(), 1.0f);
}

class IniParser {
public:
    using Section = std::map<std::string, std::string>;
    using IniData = std::map<std::string, Section>;

    static IniData parse(const std::string &filePath) {
        IniData data;
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
        std::string line, currentSection;
        while (std::getline(file, line)) {
            line = trim(line);
            if (line.empty() || line[0] == '#') {
                continue;
            }
            if (line[0] == '[' && line.back() == ']') {
                currentSection = line.substr(1, line.size() - 2);
            } else {
                auto delimiterPos = line.find(':');
                if (delimiterPos != std::string::npos) {
                    std::string key = trim(line.substr(0, delimiterPos));
                    std::string value = trim(line.substr(delimiterPos + 1));
                    data[currentSection][key] = value;
                }
            }
        }
        return data;
    }

private:
    static std::string trim(const std::string &str) {
        const auto whitespace = " \t\n\r\f\v";
        const size_t start = str.find_first_not_of(whitespace);
        const size_t end = str.find_last_not_of(whitespace);
        return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
    }
};

void Configs::init() {
    IniParser parser; {
        int index = 0;
        auto *unit = new MetaUnit();
        for (IniParser::IniData data = IniParser::parse("../M2A3/M2布雷德利.ini"); const auto &[section_id, snd]: data) {
            if (section_id == "core") {
                for (const auto &[fst, snd]: snd) {
                    if (fst == "name")unit->name = snd;
                    else if (fst == "displayText")unit->displayText = snd;
                    else if (fst == "displayDescription")unit->displayDescription = snd;
                    else if (fst == "price")unit->price = std::stoi(snd);
                    else if (fst == "maxHp")unit->maxHp = std::stoi(snd);
                    else if (fst == "techLevel")unit->techLevel = std::stoi(snd);
                    else if (fst == "buildSpeed")unit->buildSpeed = std::stof(snd);
                    else if (fst == "energyMax")unit->energyMax = std::stof(snd);
                    else if (fst == "energyRegen")unit->energyRegen = std::stof(snd);
                    else if (fst == "mass")unit->mass = std::stoi(snd);
                    else if (fst == "radius")unit->radius = std::stoi(snd);
                    else if (fst == "displayRadius")unit->displayRadius = std::stoi(snd);
                    else if (fst == "fogOfWarSightRange")unit->fogOfWarSightRange = std::stoi(snd);
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
                    else if (fst == "exit_x")unit->exit_x = std::stof(snd);
                    else if (fst == "exit_y")unit->exit_y = std::stof(snd);
                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
            } else if (section_id == "graphics") {
                for (const auto &[fst, snd]: snd) {
                    if (fst == "total_frames")unit->total_frames = std::stoi(snd);
                    else if (fst == "image")unit->image = snd;
                    else if (fst == "image_wreak")unit->image_wreak = snd;
                    else if (fst == "scale")unit->scale = std::stof(snd);
                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
            } else if (section_id == "attack") {
                for (const auto &[fst, snd]: snd) {
                    if (fst == "canAttack")unit->canAttack = snd == "true";
                    else if (fst == "canAttackFlyingUnits")unit->canAttackFlyingUnits = snd == "true";
                    else if (fst == "canAttackLandUnits")unit->canAttackLandUnits = snd == "true";
                    else if (fst == "canAttackUnderwaterUnits")unit->canAttackUnderWaterUnits = snd == "true";
                    else if (fst == "shootDelay")unit->shootDelay = std::stof(snd);
                    else if (fst == "maxAttackRange")unit->maxAttackRange = std::stof(snd);
                    else if (fst == "showRangeUIGuide")unit->showRangeUIGuide = snd == "true";
                    else if (fst == "turretMultiTargeting")unit->turretMultiTargeting = snd == "true";
                    else if (fst == "turretSize")unit->turretSize = std::stof(snd);
                    else if (fst == "turretTurnSpeed")unit->turretTurnSpeed = std::stof(snd);
                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
            } else if (section_id == "movement") {
                for (const auto &[fst, snd]: snd) {
                    if (fst == "movementType") {
                        //TODO
                        if (snd == "land")unit->movement = LAND;
                        else if (snd == "air")unit->movement = AIR;
                        else if (snd == "hover")unit->movement = HOVER;
                        else if (snd == "sub")unit->movement = SUB;
                    } else if (fst == "moveSpeed")unit->moveSpeed = std::stof(snd);
                    else if (fst == "moveAccelerationSpeed")unit->moveAcc = std::stof(snd);
                    else if (fst == "moveDecelerationSpeed")unit->moveDec = std::stof(snd);
                    else if (fst == "maxTurnSpeed")unit->maxTurnSpeed = std::stof(snd);
                    else if (fst == "turnAcceleration")unit->turnAcc = std::stof(snd);
                    else if (fst == "targetHeight")unit->targetHeight = std::stof(snd);
                    else if (fst == "joinGroup")unit->joinGroup = snd == "true";

                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
            } else {
                if (section_id.starts_with("turret")) {
                    auto *turret = new MetaTurret();
                    // unit->turretData[section_id.substr(7)] = ;
                    for (const auto &kv: snd) {
                        if (kv.first == "barrelX")turret->barrelX = std::stof(kv.second);
                        else if (kv.first == "barrelY")turret->barrelY = std::stof(kv.second);
                        else if (kv.first == "image")turret->image = kv.second;
                        else if (kv.first == "turnSpeed")turret->turnSpeed = std::stof(kv.second);
                        else if (kv.first == "turnAcc")turret->turnAcc = std::stof(kv.second);
                        else if (kv.first == "shootSound")turret->shootSound = kv.second;
                        else if (kv.first == "shootSoundVolume")turret->shootSoundVolume = std::stof(kv.second);
                        else if (kv.first == "recoilOffset")turret->recoilOffset = std::stof(kv.second);
                        else if (kv.first == "recoilOutTime")turret->recoilOutTime = std::stof(kv.second);
                        else if (kv.first == "recoilReturnTime")turret->recoilReturnTime = std::stof(kv.second);
                        else if (kv.first == "range")turret->range = std::stof(kv.second);
                        else if (kv.first == "rangeMin")turret->rangeMin = std::stof(kv.second);
                        else if (kv.first == "delay")turret->delay = std::stof(kv.second);
                        else if (kv.first == "invisible")turret->invisible = kv.second == "true";
                        else if (kv.first == "turnSpeedAcceleration")
                            turret->turnSpeedAcceleration = std::stof(kv.second);
                        else if (kv.first == "attachedTo")turret->attachedTo = kv.second;
                        else if (kv.first == "canShoot")turret->canShoot = kv.second == "true";
                        else if (kv.first == "limitingMinRange")turret->limitingMinRange = std::stof(kv.second);
                        else if (kv.first == "projectile")turret->projectile = kv.second;
                        else if (kv.first == "slave")turret->slave = kv.second == "true";
                        else if (kv.first == "shoot_effect")turret->shoot_effect = kv.second;
                        else {
                            qDebug() << "missed key:" << kv.first << "in section:" << section_id;
                        }
                    }
                } else if (section_id.starts_with("projectile")) {
                    auto *projectile = new MetaProjectiles();
                    for (const auto &kv: snd) {
                        if (kv.first == "directDamage")projectile->directDamage = std::stof(kv.second);
                        else if (kv.first == "areaDamage")projectile->areaDamage = std::stof(kv.second);
                        else if (kv.first == "areaRadius")projectile->areaRadius = std::stof(kv.second);
                        else if (kv.first == "image")projectile->image = kv.second;
                        else if (kv.first == "frame")projectile->frame = std::stoi(kv.second);
                        else if (kv.first == "lightSize")projectile->lightSize = std::stof(kv.second);
                        else if (kv.first == "lightColor")projectile->lightColor = parseColor(kv.second);
                        else if (kv.first == "explodeEffect")projectile->explodeEffect = kv.second;
                        else if (kv.first == "life")projectile->life = std::stof(kv.second);
                        else if (kv.first == "speed")projectile->speed = std::stof(kv.second);
                        else if (kv.first == "drawSize")projectile->drawSize = std::stof(kv.second);
                        else if (kv.first == "deflectionPower")projectile->deflectionPower = std::stof(kv.second);
                        else {
                            qDebug() << "missed key:" << kv.first << "in section:" << section_id;
                        }
                    }
                    meta_projectiles[section_id.substr(10)] = projectile;
                } else if (section_id.starts_with("effect")) {
                    auto *effect = new MetaEffect();
                    for (const auto &kv: snd) {
                        if (kv.first == "totalFrames")effect->totalFrames = std::stoi(kv.second);
                        else if (kv.first == "attachedToUnit")effect->attachedToUnit = kv.second == "true";
                        else if (kv.first == "image")effect->image = kv.second;
                        else if (kv.first == "animateFrameStart")effect->animateFrameStart = std::stoi(kv.second);
                        else if (kv.first == "animateFrameEnd")effect->animateFrameEnd = std::stoi(kv.second);
                        else if (kv.first == "animateFrameDelay")effect->animateFrameDelay = std::stoi(kv.second);
                        else if (kv.first == "life")effect->life = std::stoi(kv.second);
                        else if (kv.first == "scaleFrom")effect->scaleFrom = std::stof(kv.second);
                        else if (kv.first == "scaleTo")effect->scaleTo = std::stof(kv.second);
                        else if (kv.first == "alpha")effect->alpha = std::stof(kv.second);
                        else if (kv.first == "xSpeedRelativeRandom")effect->xSpeedRelativeRandom = std::stof(kv.second);
                        else if (kv.first == "ySpeedRelativeRandom")effect->ySpeedRelativeRandom = std::stof(kv.second);
                        else if (kv.first == "hSpeed")effect->hSpeed = std::stof(kv.second);
                        else if (kv.first == "drawUnderUnit")effect->drawUnderUnit = kv.second == "true";
                        else if (kv.first == "priority") {
                            if (kv.second == "high")effect->priority = high;
                            else if (kv.second == "medium")effect->priority = medium;
                            else if (kv.second == "low")effect->priority = low;
                        } else if (kv.first == "atmospheric")effect->atmospheric = kv.second == "true";
                        else if (kv.first == "delayedStartTimer")effect->delayedStartTimer = std::stof(kv.second);
                        else if (kv.first == "alsoPlaySound") {
                            auto name_and_volume = split(kv.second, ':');
                            effect->alsoPlaySound = name_and_volume[0];
                            effect->alsoPlaySoundVolume = std::stof(name_and_volume[1]);
                        } else {
                            qDebug() << "missed key:" << kv.first << "in section:" << section_id;
                        }
                    }
                    meta_effects[section_id.substr(6)] = effect;
                } else {
                    qDebug() << "missed section:" << section_id;
                }
            }
        }
        meta_units[unit->name] = unit;
    }
}
