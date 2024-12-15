//
// Created by root on 12/13/24.
//

#include "Trigger.h"

#include "DirectPathFollowController.h"
#include "Game.h"
#include "Unit.h"
#include "UnitConfigs.h"


void Trigger::Event::update()
{
    result = false;
    if (game->time - last_active_time < delay)
    {
        return;
    }
    last_active_time = game->time;
    result = detect();
}

void Trigger::Action::update(const std::map<std::string, Trigger::Event*>& event_map)
{
    if (repeat != -1 && has_repeated >= repeat)
    {
        return;
    }
    if (game->time - last_active_time < delay)
    {
        return;
    }

    if (event_map.at(by)->result)
    {
        last_active_time = game->time;
        has_repeated++;
        execute();
    }
}

bool Trigger::UnitDetect::detect()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);

    int cnt = std::ranges::count_if(units, [this](const Unit* unit)
    {
        if (!require_type.empty() && unit->meta->name != require_type)
        {
            return false;
        }
        if (require_team != 5211324 && unit->team != require_team)
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

bool Trigger::ReachTime::detect()
{
    return game->time >= time;
}

Trigger::UnitMove::UnitMove(Game* game, QVector3D posLB, QVector3D posRT): Action(game)
{
    this->posLB = posLB;
    this->posRT = posRT;
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
        FlowField* flow_field = game->map_config.flow_fields.at(target).at(unit->meta->movement);
        if (unit->controller == nullptr)
        {
            unit->controller = new DirectPathFollowController(unit, flow_field);
        }
        else if (auto ctrl = dynamic_cast<DirectPathFollowController*>(unit->controller))
        {
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

Trigger::UnitRemove::UnitRemove(Game* game, QVector3D posLB, QVector3D posRT): Action(game)
{
    this->posLB = posLB;
    this->posRT = posRT;
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
        unit->marked_for_delete = true;
    }
}

Trigger::UnitAdd::UnitAdd(Game* game, const QVector3D pos): Action(game)
{
    posLB = pos;
}

void Trigger::UnitAdd::execute()
{
    for (const auto& unit : units)
    {
        auto u = new Unit(game, UnitConfigs::meta_units.at(unit), team, posLB, 0);
        u->rotation=rot;
        u->linear_velocity.setX(vx);
        u->linear_velocity.setY(vy);
        game->units.push_back(u);
    }
}

void Trigger::Dialog::execute()
{
    game->battle_field_widget->dialog(message);
    game->pause();
}

void Trigger::Info::execute()
{
    game->battle_field_widget->info(message);
}
