//
// Created by root on 12/13/24.
//

#include "Trigger.h"
#include "Game.h"
#include "Unit.h"


void Trigger::Event::update()
{
    result = false;
    if (game->time < delay)
    {
        return;
    }
    result = detect();
}

void Trigger::Action::update(const std::map<std::string, Trigger::Event*>& event_map)
{
    if (event_map.at(activeBy)->result)execute();
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

Trigger::Move::Move(Game* game, QVector3D posLB, QVector3D posRT): Action(game), posLB(posLB), posRT(posRT)

{
}

void Trigger::Move::execute()
{
    auto units = game->grids_manager.scan_units(posLB, posRT);
    for (auto unit : units)
    {
        // auto flock = new Flock(game);
        // flock->boids.insert(unit);
        // flock->flow_field=game->map_config.flow_fields.at(target);
        // game->flocks.push_back(flock);
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
