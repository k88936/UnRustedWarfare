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
        bool each = false;
        bool result = false;
        Game* game;
    };

    class Action
    {
    public:
        virtual ~Action() = default;
        std::vector<std::string> by;
        bool by_all = false;
        QVector3D posLB;
        QVector3D posRT;
        float delay = 0;
        float warmup = 0;
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
        virtual void  update(const std::map<std::string, Trigger::Event*>& event_map);
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

        UnitDetect(Game* game, QVector3D lb, QVector3D RT): Event(game, lb, RT)
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

    class UnitFilt : public Trigger::Action
    {
    public:
        std::set<std::string> require_type;
        int require_team = 5211324;

        UnitFilt(Game* game, QVector3D posLB, QVector3D posRT): Action(game)
        {
            this->posLB = posLB;
            this->posRT = posRT;
        }

        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class UnitMove : public Trigger::UnitFilt
    {
    public:
        std::string target;

        UnitMove(Game* game, QVector3D posLB, QVector3D posRT): UnitFilt(game, posLB, posRT)
        {
        };
        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class UnitRemove : public Trigger::UnitFilt
    {
    public:
        UnitRemove(Game* game, QVector3D posLB, QVector3D posRT): UnitFilt(game, posLB, posRT)
        {
        };
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
        std::set<std::string> tags;
        /*
         * "__protect__" : game over if this unit dies
         */

        UnitAdd(Game* game, const QVector3D pos): Action(game)
        {
            posLB = pos;
        }

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

    class CamSet : public Trigger::Action
    {
    public:
        float zoom = -1;

        CamSet(Game* game, QVector3D posLB, QVector3D posRT): Action(game)
        {
            target_ = (posLB + posRT) / 2;
        }

        void parse(const tmx::Property& property) override;
        void execute() override;

    private:
        QVector3D target_;
    };

    class CamMove : public Trigger::Action
    {
    public:
        float soft = 0.1;

        float zoom = -1;

        CamMove(Game* game, QVector3D posLB, QVector3D posRT): Action(game)
        {
            target_ = (posLB + posRT) / 2;
        }

        void parse(const tmx::Property& property) override;
        void execute() override;

    private:
        QVector3D target_;
    };

    class UnitChange : public Trigger::UnitFilt
    {
    public:
        int team_to = 5211324;
        std::set<std::string> tags_add;
        std::set<std::string> tags_remove;

        UnitChange(Game* game, QVector3D posLB, QVector3D posRT): UnitFilt(game, posLB, posRT)
        {
        }

        void parse(const tmx::Property& property) override;
        void execute() override;
    };

    class Win : public Trigger::Action
    {
    public:
        explicit Win(Game* game): Action(game)
        {
        }

        void parse(const tmx::Property& property) override;
        void execute() override;


    };

    class Lose : public Trigger::Action
    {
    public:
        explicit Lose(Game* game): Action(game)
        {
        }

        void parse(const tmx::Property& property) override;
        void execute() override;
    };
};


#endif //TRIGGER_H
