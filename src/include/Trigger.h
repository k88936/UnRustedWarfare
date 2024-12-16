//
// Created by root on 12/13/24.
//

#ifndef TRIGGER_H
#define TRIGGER_H
#include <map>
#include <QVector3D>
#include <set>
#include <string>


namespace tmx
{
    class Property;
}

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

        virtual void parse(const tmx::Property& property);

        void update();
        float last_active_time = 0;
        float delay = 0;
        bool result = false;
        Game* game;
    };

    class Action
    {
    public:
        std::vector<std::string> by;
        bool by_all = false;
        QVector3D posLB;
        QVector3D posRT;
        float delay = 0;
        float last_active_time = 0;

        int repeat = 1; //-1 for infinite
        int has_repeated = 0;

        explicit Action(Game* game): game(game)
        {
        };

        virtual void execute()
        {
        }

        virtual void parse(const tmx::Property& property);

        void update(const std::map<std::string, Trigger::Event*>& event_map);
        Game* game;
    };

    class UnitDetect : public Trigger::Event
    {
    public:
        std::set<std::string> require_type;
        int require_cnt_min = -1;
        int require_cnt_max = -1;
        int require_team = 5211324;
        bool detect() override;

        UnitDetect(Game* game, QVector3D left, QVector3D top): Event(game, left, top)
        {
        };
        void parse(const tmx::Property& property) override;
    };

    class ReachTime : public Trigger::Event
    {
    public:
        float time = 0;

        explicit ReachTime(Game* game): Event(game)
        {
        }

        bool detect() override;
        void parse(const tmx::Property& property) override;
    };

    class UnitMove : public Trigger::Action
    {
    public:
        std::set<std::string> require_type;
        std::string target;
        int require_team = 5211324;
        UnitMove(Game* game, QVector3D posLB, QVector3D posRT);
        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class UnitRemove : public Trigger::Action
    {
    public:
        int require_team = 5211324;
        std::set<std::string> require_type;
        UnitRemove(Game* game, QVector3D posLB, QVector3D posRT);
        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class UnitAdd : public Trigger::Action
    {
    public:
        std::vector<std::string> units;
        float rot = 0;
        int team = 0;
        float vx = 0, vy = 0;
        UnitAdd(Game* game, QVector3D pos);
        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class Dialog : public Trigger::Action
    {
    public:
        explicit Dialog(Game* game): Action(game)
        {
        }

        std::string message;
        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class Info : public Trigger::Action
    {
    public:
        explicit Info(Game* game): Action(game)
        {
        }

        std::string message;
        void parse(const tmx::Property& property) override;
        void execute() override;
    };
};


#endif //TRIGGER_H
