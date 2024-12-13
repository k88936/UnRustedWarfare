//
// Created by root on 12/13/24.
//

#ifndef TRIGGER_H
#define TRIGGER_H
#include <map>
#include <QVector3D>
#include <string>



class Game;

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
        bool result = false;
        Game* game;
    };

    class Action
    {
    public:
        std::string activeBy;
        // QVector3D posLB;
        // QVector3D posRT;

        explicit Action(Game* game): game(game)
        {
        };

        // explicit Action(Game* game, QVector3D posLB,QVector3D posRT): game(game), posLB(posLB), posRT(posRT)
        // {
        // }


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

    class Move : public Trigger::Action
    {
    public:
        // FlowField* flow_field = nullptr;
        std::string target;
        Move(Game* game, QVector3D posLB, QVector3D posRT);
        void execute() override;
        QVector3D posLB, posRT;
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
