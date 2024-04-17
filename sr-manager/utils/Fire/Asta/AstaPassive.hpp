#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/TurnBegin.hpp"
#include "../../../battle/TurnEnd.hpp"
#include "../../../battle/Die.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../../battle/MpUp.hpp"

namespace sr {

    // 艾丝妲被动1：天象学
    // 每击中1名不同的敌人获得1点技力。
    // 每拥有1点技力，艾丝妲防御提高1点，我方全体攻击提高1点。
    // 回合开始时消耗3点技力。
    
    // 实现：击中回复在主动技能已经实现
    // 回合开始时：消耗技力并记录当前的技力值
    // 回合结束时：根据技力值变化调整属性
    // 死亡时：清空技力值并调整属性
    struct AstaP1A : public Trigger {
        int sp_at_turn_begin = 0;

        AstaP1A(BattleUnit& owner) :
            Trigger(owner, TriggerTime::TurnBegin) {}

        const char* name() const override {
            return "天象学";
        }

        std::string description() const override { return ""; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            return &turn_begin.target == &target;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_begin = dynamic_cast<TurnBegin&>(procedure);
            // 不显示在战报中
            int old_sp = target.sp;
            int delta = target.star >= 6 ? 2 : 3;
            sp_at_turn_begin = std::max(0, old_sp - delta);
            int actual_delta = old_sp - sp_at_turn_begin;
            if (actual_delta > 0) {
                // 调整属性
                target.defense -= actual_delta;
                for (auto& u : turn_begin.battle.teammates(target)) {
                    if (u->alive) {
                        u->attack -= actual_delta;
                    }
                }
            }
            target.sp = sp_at_turn_begin;
        }
    };

    // 第二触发器，回合结束
    struct AstaP1B : public Trigger {
        AstaP1A& p1a;

        AstaP1B(AstaP1A& p1a) :
            Trigger(p1a.target, TriggerTime::TurnEnd), p1a(p1a) {}

        const char* name() const override {
            return "天象学";
        }

        std::string description() const override { return ""; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_end = dynamic_cast<const TurnEnd&>(procedure);
            return &turn_end.target == &target;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_end = dynamic_cast<TurnEnd&>(procedure);
            int old_sp = p1a.sp_at_turn_begin;
            int actual_delta = target.sp - old_sp;
            if (actual_delta > 0) {
                // 调整属性
                target.defense += actual_delta;
                for (auto& u : turn_end.battle.teammates(target)) {
                    if (u->alive) {
                        u->attack += actual_delta;
                    }
                }
            }
        }
    };

    // 第三触发器，死亡
    struct AstaP1C : public Trigger {

        AstaP1C(BattleUnit& owner) :
            Trigger(owner, TriggerTime::Die) {}

        const char* name() const override {
            return "天象学";
        }

        std::string description() const override { return ""; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& die = dynamic_cast<const Die&>(procedure);
            return &die.target == &target;
        }

        void on_trigger(Procedure& procedure) override {
            auto& die = dynamic_cast<Die&>(procedure);
            // 调整属性
            for (auto& u : die.battle.teammates(target)) {
                if (u->alive) {
                    u->attack -= target.sp;
                }
            }
            target.sp = 0;
        }
    };

    // 艾丝妲被动2：灵光一现
    // 进入战斗时，对敌方全体削韧4。
    struct AstaP2 : public Trigger {
        AstaP2(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "灵光一现";
        }

        std::string description() const override {
            return "进入战斗时，对敌方全体削韧4。";
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                for (auto& unit : game_start.battle.opponents(target)) {
                    ReduceToughness{ game_start.battle, target, *unit,
                        { }, 4, Type::Fire }.invoke();
                }
                });
        }
    };

    // 艾丝妲 4星被动：极光显现之时
    // 回合结束时如果技力满，回复1点能量。
    struct AstaS4 : public Trigger {
        
        AstaS4(BattleUnit& owner) :
            Trigger(owner, TriggerTime::TurnEnd) {}

        const char* name() const override {
            return "极光显现之时";
        }

        std::string description() const override { return ""; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_end = dynamic_cast<const TurnEnd&>(procedure);
            return &turn_end.target == &target && target.sp == target.max_sp;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_end = dynamic_cast<TurnEnd&>(procedure);
            turn_end.on_trigger(*this, [&] {
                MpUp{ turn_end.battle, target, {}, 1 }.invoke();
                });
        }
    };
}