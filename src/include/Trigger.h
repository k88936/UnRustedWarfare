//
// Created by root on 12/13/24.
//

#ifndef TRIGGER_H
#define TRIGGER_H
#include <map>
#include <QVector3D>
#include <string>


class Game;

/**
 *
 *
 */
namespace Trigger
{
    class Event
    {
    public:
        QVector3D posLB;
        QVector3D posRT;

        explicit Event(Game* game): game(game)
        {
        };

        explicit Event(Game* game, QVector3D posLB, QVector3D posRT): game(game), posLB(posLB), posRT(posRT)
        {
        }

        virtual bool detect()
        {
            return false;
        }

        void update();
        float delay = 0;
        float last_active_time = 0;
        bool result = false;
        Game* game;
    };

    class Action
    {
    public:
        std::string activeBy;
        QVector3D posLB;
        QVector3D posRT;
        float delay = 0;
        float last_active_time = 0;

        int max_repeat = -1; //-1 for infinite
        int has_repeated = 0;

        explicit Action(Game* game): game(game)
        {
        };

        virtual void execute()
        {
        }

        void update(const std::map<std::string, Trigger::Event*>& event_map);
        Game* game;
    };

    class UnitDetect : public Trigger::Event
    {
    public:
        std::string require_type;
        int require_cnt_min = -1;
        int require_cnt_max = -1;
        int require_team = 5211324;
        bool detect() override;

        UnitDetect(Game* game, QVector3D left, QVector3D top): Event(game, left, top)
        {
        };
    };

    class ReachTime : public Trigger::Event
    {
    public:
        float time = 0;

        explicit ReachTime(Game* game): Event(game)
        {
        }

        bool detect() override;
    };

    class UnitMove : public Trigger::Action
    {
    public:
        // FlowField* flow_field = nullptr;
        std::string target;
        int require_team = 5211324;
        UnitMove(Game* game, QVector3D posLB, QVector3D posRT);
        void execute() override;
    };

    class UnitRemove : public Trigger::Action
    {
    public:
        int require_team = 5211324;
        UnitRemove(Game* game, QVector3D posLB, QVector3D posRT);
        void execute() override;
    };

    class UnitAdd : public Trigger::Action
    {
    public:
        std::vector<std::string> units;
        int team = 0;
        UnitAdd(Game* game, QVector3D pos);
        void execute() override;
    };

    class Dialog : public Trigger::Action
    {
    public:
        Dialog(Game* game, const std::string& message): Action(game), message(message)
        {
        }

        std::string message;
        void execute() override;
    };

    class Info : public Trigger::Action
    {
    public:
        Info(Game* game, const std::string& message): Action(game), message(message)
        {
        }

        std::string message;
        void execute() override;
    };
};


#endif //TRIGGER_H
