//
// Created by root on 11/6/24.
//
#include <QDir>

#include <fstream>
#include <map>
#include <QColor>
#include <string>
#include "Configs.h"

#include <qimage.h>

#include "Unit.h"
#include "../RenderEngine.h"

std::unordered_map<std::string, MetaUnit *> Configs::meta_units;
std::unordered_map<std::string, MetaProjectiles *> Configs::meta_projectiles;
std::unordered_map<std::string, MetaEffect *> Configs::meta_effects;
std::unordered_map<std::string, QImage> Configs::images;

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
    return {color.redF(), color.greenF(), color.blueF(), 1.0f};
}

class IniParser {
public:
    using Section = std::map<std::string, std::string>;
    using IniData = std::map<std::string, Section>;

    static IniData parse(const QString &filePath) {
        IniData data;
        QFile file(filePath);

        if (!file.open(QFile::ReadOnly)) {
            throw std::runtime_error("Could not open file");
        }
        QTextStream in{&file};
        std::string line, currentSection;
        while (!in.atEnd()) {
            line = in.readLine().toStdString();
            if (line.empty()) {
                continue;
            }
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

void Configs::loadIni(const QString &path) {
    const float rw2sw = 1.0 / 40;
    // int index = 0;
    auto *unit = new MetaUnit();
    std::unordered_map<std::string, MetaTurret *> turretData;
    IniParser::IniData data = IniParser::parse(path);
    for (const auto &
         [section_id, snd]: data) {
        if (section_id.starts_with("turret")) {
            auto *turret = new MetaTurret();
            turretData[section_id.substr(7)] = turret;
        } else if (section_id.starts_with("projectile")) {
            auto *projectile = new MetaProjectiles();
            meta_projectiles[section_id.substr(10)] = projectile;
        } else if (section_id.starts_with("effect")) {
            auto *effect = new MetaEffect();
            meta_effects[section_id.substr(7)] = effect;
        }
    }
    for (const auto &[section_id, content]: data) {
        if (section_id == "core") {
            for (const auto &[fst, snd]: content) {
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
                else if (fst == "radius")unit->radius = std::stof(snd) * rw2sw;
                else if (fst == "displayRadius")unit->displayRadius = std::stof(snd) * rw2sw;
                else if (fst == "fogOfWarSightRange")unit->fogOfWarSightRange = std::stof(snd) * rw2sw;
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
                else if (fst == "exit_x")unit->exit_y = std::stof(snd) * rw2sw;
                else if (fst == "exit_y")unit->exit_x = std::stof(snd) * rw2sw;
                else {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
        } else if (section_id == "graphics") {
            for (const auto &[fst, snd]: content) {
                if (fst == "total_frames")unit->total_frames = std::stoi(snd);
                else if (fst == "image") {
                    unit->image = snd;
                } else if (fst == "image_wreak")unit->image_wreak = snd;
                else if (fst == "scale")unit->scale = std::stof(snd) * rw2sw;
                else {
                    qDebug() << "missed key:" << fst << "in section:" << section_id;
                }
            }
        } else if (section_id == "attack") {
            for (const auto &[fst, snd]: content) {
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
            for (const auto &[fst, snd]: content) {
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
                auto *turret = turretData[section_id.substr(7)];
                bool attachToUnit = true;
                for (const auto &[fst, snd]: content) {
                    if (fst == "barrelX")turret->barrelX = std::stof(snd);
                    else if (fst == "barrelY")turret->barrelY = std::stof(snd);
                    else if (fst == "x")turret->y = std::stof(snd) * rw2sw;
                    else if (fst == "y")turret->x = std::stof(snd) * rw2sw;
                    else if (fst == "image")turret->image = snd;
                    else if (fst == "turnSpeed")turret->turnSpeed = std::stof(snd);
                    else if (fst == "turnAcc")turret->turnAcc = std::stof(snd);
                    else if (fst == "shootSound")turret->shootSound = snd;
                    else if (fst == "shootSoundVolume")turret->shootSoundVolume = std::stof(snd);
                    else if (fst == "recoilOffset")turret->recoilOffset = std::stof(snd);
                    else if (fst == "recoilOutTime")turret->recoilOutTime = std::stof(snd);
                    else if (fst == "recoilReturnTime")turret->recoilReturnTime = std::stof(snd);
                    else if (fst == "range")turret->range = std::stof(snd) * rw2sw;
                    else if (fst == "rangeMin")turret->rangeMin = std::stof(snd) * rw2sw;
                    else if (fst == "invisible") {
                        turret->invisible = snd == "true";
                    } else if (fst == "delay") {
                        turret->delay = std::stof(snd);
                    } else if (fst == "turnSpeedAcceleration")turret->turnSpeedAcceleration = std::stof(snd);
                    else if (fst == "attachedTo") {
                        turret->attachedTo = snd;
                        attachToUnit = false;
                    } else if (fst == "canShoot")turret->canShoot = snd == "true";
                    else if (fst == "limitingMinRange")turret->limitingMinRange = std::stof(snd) * rw2sw;
                    else if (fst == "projectile")turret->projectile = snd;
                    else if (fst == "slave")turret->slave = snd == "true";
                    else if (fst == "shoot_effect")turret->shoot_effect = snd;
                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                    turret->slot_translation=QVector3D(turret->x,turret->y,0);
                    turret->slot_isFixed=false;
                    turret->slot_inVisible=turret->invisible;
                if (attachToUnit) {
                    unit->attached_turret.push_back(turret);
                } else {
                    turretData[turret->attachedTo]->attached_turrets.push_back(turret);
                }
                if(turret->turnSpeed==-1){
                    turret->turnSpeed=unit->turretTurnSpeed;
                }
            } else if (section_id.starts_with("projectile")) {
                auto *projectile = meta_projectiles[section_id.substr(10)];
                for (const auto &[fst, snd]: content) {
                    if (fst == "directDamage")projectile->directDamage = std::stof(snd);
                    else if (fst == "areaDamage")projectile->areaDamage = std::stof(snd);
                    else if (fst == "areaRadius")projectile->areaRadius = std::stof(snd) * rw2sw;
                    else if (fst == "image")projectile->image = snd;
                    else if (fst == "frame")projectile->frame = std::stoi(snd);
                    else if (fst == "lightSize")projectile->lightSize = std::stof(snd);
                    else if (fst == "lightColor")projectile->lightColor = parseColor(snd);
                    else if (fst == "explodeEffect")projectile->explodeEffect = snd;
                    else if (fst == "life")projectile->life = std::stof(snd);
                    else if (fst == "speed")projectile->speed = std::stof(snd);
                    else if (fst == "drawSize")projectile->drawSize = std::stof(snd);
                    else if (fst == "deflectionPower")projectile->deflectionPower = std::stof(snd);
                    else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
                    }
                }
                meta_projectiles[section_id.substr(10)] = projectile;
            } else if (section_id.starts_with("effect")) {
                auto *effect = meta_effects[section_id.substr(7)];
                for (const auto &[fst, snd]: content) {
                    if (fst == "totalFrames")effect->totalFrames = std::stoi(snd);
                    else if (fst == "attachedToUnit")effect->attachedToUnit = snd == "true";
                    else if (fst == "image")effect->image = snd;
                    else if (fst == "animateFrameStart")effect->animateFrameStart = std::stoi(snd);
                    else if (fst == "animateFrameEnd")effect->animateFrameEnd = std::stoi(snd);
                    else if (fst == "animateFrameDelay")effect->animateFrameDelay = std::stoi(snd);
                    else if (fst == "life")effect->life = std::stoi(snd);
                    else if (fst == "scaleFrom")effect->scaleFrom = std::stof(snd) * rw2sw;
                    else if (fst == "scaleTo")effect->scaleTo = std::stof(snd) * rw2sw;
                    else if (fst == "alpha")effect->alpha = std::stof(snd);
                    else if (fst == "xSpeedRelativeRandom")effect->xSpeedRelativeRandom = std::stof(snd);
                    else if (fst == "ySpeedRelativeRandom")effect->ySpeedRelativeRandom = std::stof(snd);
                    else if (fst == "hSpeed")effect->hSpeed = std::stof(snd);
                    else if (fst == "drawUnderUnit")effect->drawUnderUnit = snd == "true";
                    else if (fst == "priority") {
                        if (snd == "high")effect->priority = high;
                        else if (snd == "medium")effect->priority = medium;
                        else if (snd == "low")effect->priority = low;
                    } else if (fst == "atmospheric")effect->atmospheric = snd == "true";
                    else if (fst == "delayedStartTimer")effect->delayedStartTimer = std::stof(snd);
                    else if (fst == "alsoPlaySound") {
                        auto name_and_volume = split(snd, ':');
                        effect->alsoPlaySound = name_and_volume[0];
                        effect->alsoPlaySoundVolume = std::stof(name_and_volume[1]);
                    } else {
                        qDebug() << "missed key:" << fst << "in section:" << section_id;
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

void Configs::init() {
    QString path = "../M2A3/";
    QDir dir(path);
    dir.setFilter(QDir::Files);
    IniParser parser;
    QStringList files = dir.entryList();
    for (const auto &file_name: files) {
        if (file_name.endsWith(".ini")) {
            loadIni(path + file_name);
        } else if (file_name.endsWith(".png")) {
            images[file_name.toStdString()] = QImage(path + file_name);
            // RenderEngine::resisterTexture(file_name.toStdString(), QImage(path + file_name));
        }
    }
}
