//
// Created by root on 12/13/24.
//

#include "Trigger.h"

#include "DirectPathFollowController.h"
#include "Game.h"
#include "Unit.h"
#include "UnitConfigs.h"


void Trigger::Event::parse(const tmx::Property& property)
{
    if (property.getName() == "delay")
    {
        delay = std::stof(property.getStringValue()) / 1000;
    }
    else if (property.getName() == "each")
    {
        each = property.getStringValue() == "true";
    }
    else
    {
        qDebug() << "unknown property: " << property.getName();
    }
}

void Trigger::Event::update()
{
    if (!each && result)
    {
        return;
    }
    result = false;
    if (game->time - last_active_time < delay)
    {
        return;
    }
    last_active_time = game->time;
    result = detect();
}

void Trigger::Action::parse(const tmx::Property& property)
{
    if (property.getName() == "by")
    {
        utils::parse_item_list(property.getStringValue(), by);
    }
    else if (property.getName() == "by_all")
    {
#ifdef DEBUG
        assert(property.getStringValue()=="true"||property.getStringValue()=="false");
#endif
        by_all = property.getStringValue() == "true";
    }
    else if (property.getName() == "delay")
    {
        delay = std::stof(property.getStringValue()) / 1000;
    }
    else if (property.getName() == "warmup")
    {
        warmup = std::stof(property.getStringValue()) / 1000;
    }
    else if (property.getName() == "repeat")
    {
        repeat = std::stoi(property.getStringValue());
    }
    else
    {
        qDebug() << "unknown property: " << property.getName();
    }
}

void Trigger::Action::update(const std::map<std::string, Trigger::Event*>& event_map)
{
    // if (warmup > 0)
    // {
    //     if (has_repeated == 0)
    //     {
    //         last_active_time = game->time;
    //         has_repeated++;
    //         return;
    //     }
    //     if (game->time - last_active_time < warmup)
    //     {
    //         return;
    //     }
    //     has_repeated = 0;
    // }
    if (repeat != -1 && has_repeated >= repeat)
    {
        return;
    }

    if (game->time - last_active_time < delay)
    {
        return;
    }


    bool flag;

    if (by_all)
    {
        flag = true;
        for (auto id : by)
        {
            if (!event_map.at(id)->result)
            {
                flag = false;
                break;
            }
        }
    }
    else
    {
        flag = false;
        for (auto id : by)
        {
            if (event_map.at(id)->result)
            {
                flag = true;
                break;
            }
        }
    }

    if (flag)
    {
        last_active_time = game->time;
        has_repeated++;
        execute();
    }
}

bool Trigger::UnitDetect::detect()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);

    const long cnt = std::ranges::count_if(units, [this](const Unit* unit)
    {
        if (!require_type.empty() && !require_type.contains(unit->meta->name))
        {
            return false;
        }
        if (require_team != 5211324 && unit->team != require_team)
        {
            return false;
        }
        if (!require_type.empty() && !require_type.contains(unit->meta->name))
        {
            return false;
        }
        return true;
    });
    if (require_cnt_min != -1 && cnt < require_cnt_min)
    {
        return false;
    }
    if (require_cnt_max != -1 && cnt > require_cnt_max)
    {
        return false;
    }

    return true;
}

void Trigger::UnitDetect::parse(const tmx::Property& property)
{
    if (property.getName() == "type")
    {
        utils::parse_item_set(property.getStringValue(), require_type);
#ifdef DEBUG
        for (auto type : require_type)
        {
            assert(UnitConfigs::meta_units.contains(type));
        }
#endif
    }
    else if (property.getName() == "min")
    {
        require_cnt_min = std::stoi(property.getStringValue());
    }
    else if (property.getName() == "max")
    {
        require_cnt_max = std::stoi(property.getStringValue());
    }
    else if (property.getName() == "team")
    {
        require_team = std::stoi(property.getStringValue());
    }
    else
        Event::parse(property);
}

bool Trigger::ReachTime::detect()
{
    return game->time >= time;
}

void Trigger::ReachTime::parse(const tmx::Property& property)
{
    if (property.getName() == "time")
    {
        time = std::stof(property.getStringValue()) / 1000;
    }
    else
        Event::parse(property);
}


void Trigger::UnitFilt::parse(const tmx::Property& property)
{
    if (property.getName() == "team")
    {
        require_team = std::stoi(property.getStringValue());
    }
    else if (property.getName() == "type")
    {
        utils::parse_item_set(property.getStringValue(), require_type);
#ifdef DEBUG
        for (auto type : require_type)
        {
            assert(UnitConfigs::meta_units.contains(type));
        }
#endif
    }
    else
        Action::parse(property);
}

void Trigger::UnitFilt::execute()
{
}

void Trigger::UnitMove::parse(const tmx::Property& property)
{
    if (property.getName() == "target")
    {
        target = property.getStringValue();
    }
    else
        UnitFilt::parse(property);
}


void Trigger::UnitMove::execute()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);
    for (auto unit : units)
    {
        if (unit->marked_for_delete)continue;
        if (require_team != 5211324 && unit->team != require_team)
        {
            continue;
        }
        if (!require_type.empty() && !require_type.contains(unit->meta->name))
        {
            continue;
        }
        FlowField* flow_field = game->map_config.flow_fields.at(target).at(unit->meta->movement);
        if (unit->controller == nullptr)
        {
            unit->controller = new DirectPathFollowController(unit, flow_field);
        }
        else if (auto ctrl = dynamic_cast<DirectPathFollowController*>(unit->controller))
        {
            ctrl->finish = false;
            ctrl->flow_field = flow_field;
        }
        else
        {
            unit->controller = new DirectPathFollowController(unit, flow_field);
        }
        // auto flock = new Flock(game);
        // flock->boids.insert(unit);
        // flock->flow_field=game->map_config.flow_fields.at(target);
        // game->flocks.push_back(flock);
    }
}


void Trigger::UnitRemove::parse(const tmx::Property& property)
{
    UnitFilt::parse(property);
}

void Trigger::UnitRemove::execute()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);
    for (auto unit : units)
    {
        if (require_team != 5211324 && unit->team != require_team)
        {
            continue;
        }
        if (!require_type.empty() && !require_type.contains(unit->meta->name))
        {
            continue;
        }
        unit->marked_for_delete = true;
    }
}


void Trigger::UnitAdd::parse(const tmx::Property& property)
{
    if (property.getName() == "team")
    {
        team = std::stoi(property.getStringValue());
    }
    else if (property.getName() == "units")
    {
        utils::parse_item_list(property.getStringValue(), units);
#ifdef DEBUG
        for (auto type : units)
        {
            assert(UnitConfigs::meta_units.contains(type));
        }
#endif
    }
    else if (property.getName() == "rot")
    {
        rot = std::stof(property.getStringValue());
    }
    else if (property.getName() == "vx")
    {
        vx = std::stof(property.getStringValue());
    }
    else if (property.getName() == "vy")
    {
        vy = std::stof(property.getStringValue());
    }
    else
        Action::parse(property);
}

void Trigger::UnitAdd::execute()
{
    if (units.size() == 1)
    {
        auto u = new Unit(game, UnitConfigs::meta_units.at(units.at(0)), team, posLB, 0);
        u->rotation = rot;
        u->linear_velocity.setX(vx);
        u->linear_velocity.setY(vy);
        game->units.push_back(u);
    }
    else

        for (const auto& unit : units)
        {
            auto u = new Unit(game, UnitConfigs::meta_units.at(unit), team,
                              posLB + utils::generate_random_small_vector(0.5), 0);
            u->rotation = rot;
            u->linear_velocity.setX(vx);
            u->linear_velocity.setY(vy);
            game->units.push_back(u);
        }
}


void Trigger::Dialog::parse(const tmx::Property& property)
{
    if (property.getName() == "msg")
    {
        message = property.getStringValue();
    }
    else
        Action::parse(property);
}

void Trigger::Dialog::execute()
{
    game->battle_field_widget->dialog(message);
    game->pause();
}


void Trigger::Info::parse(const tmx::Property& property)
{
    if (property.getName() == "msg")
    {
        message = property.getStringValue();
    }
    else
        Action::parse(property);
}

void Trigger::Info::execute()
{
    game->battle_field_widget->info(message);
}

void Trigger::CamSet::parse(const tmx::Property& property)
{
    if (property.getName() == "zoom")
    {
        zoom = std::stof(property.getStringValue());
    }
    else
        Action::parse(property);
}

void Trigger::CamSet::execute()
{
    game->battle_field_widget->camera_pos.setX(target_.x());
    game->battle_field_widget->camera_pos.setY(target_.y());
    if (zoom != -1)
    {
        game->battle_field_widget->camera_zoom = zoom;
    }
    game->battle_field_widget->update_camera();
}

void Trigger::CamMove::parse(const tmx::Property& property)
{
    if (property.getName() == "soft")
    {
        soft = std::stof(property.getStringValue());
    }
    else if (property.getName() == "zoom")
    {
        zoom = std::stof(property.getStringValue());
    }
    else
        Action::parse(property);
}

void Trigger::CamMove::execute()
{
    if (has_repeated == 0)
    {
        has_repeated = 1;
        return;
    }
    game->battle_field_widget->camera_pos.setX(
        utils::soft_approach(game->battle_field_widget->camera_pos.x(), target_.x(), soft));
    game->battle_field_widget->camera_pos.setY(
        utils::soft_approach(game->battle_field_widget->camera_pos.y(), target_.y(), soft));
    if (zoom != -1)
    {
        game->battle_field_widget->camera_zoom = utils::soft_approach(game->battle_field_widget->camera_zoom, zoom,
                                                                      soft);
    }
    game->battle_field_widget->update_camera();
}

void Trigger::UnitChange::parse(const tmx::Property& property)
{
    if (property.getName() == "team_to")
    {
        team_to = std::stoi(property.getStringValue());
    }

    else
        UnitFilt::parse(property);
}

void Trigger::UnitChange::execute()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);
    for (auto unit : units)
    {
        if (require_team != 5211324 && unit->team != require_team)
        {
            continue;
        }
        if (!require_type.empty() && !require_type.contains(unit->meta->name))
        {
            continue;
        }
        if (team_to != 5211324)
        {
            unit->team = team_to;
        }
    }
}

void Trigger::Win::parse(const tmx::Property& property)
{
    Action::parse(property);
}

void Trigger::Win::execute()
{
    game->battle_field_widget->game_end(true);
    Action::execute();
}

void Trigger::Lose::parse(const tmx::Property& property)
{
    Action::parse(property);
}

void Trigger::Lose::execute()
{
    game->battle_field_widget->game_end(false);
    Action::execute();
}
