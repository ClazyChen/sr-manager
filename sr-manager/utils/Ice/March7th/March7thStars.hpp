#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../effects/Shield.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../battle/TurnBegin.hpp"
#include "../../../battle/HpUp.hpp"

namespace sr {

    // 三月七2星被动：记忆中的它
    // 进入战斗时，为生命值最低的我方角色附加16点护盾（3回合）。
    struct March7thS2 : public Trigger {
        int shield = 16;
        int duration = 3;

        March7thS2(BattleUnit& owner)
            : Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "记忆中的它";
        }

        std::string description() const override {
            return std::format("进入战斗时，为生命值最低的我方角色附加{}点护盾（{}回合）。", shield, duration);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            auto& game_start = dynamic_cast<GameStart&>(procedure);
            auto& battle = game_start.battle;
            int hp = std::numeric_limits<int>::max();
            BattleUnit* u = nullptr;
            for (auto& unit : battle.teammates(target)) {
                if (unit->alive) {
                    if (unit->hp < hp) {
                        hp = unit->hp;
                        u = unit.get();
                    }
                }
            }
            if (u) {
                AddEffect{ game_start.battle, target, *u,
                    { }, std::make_unique<Shield>(duration, target, shield * 10) }.invoke();
            }
        }
    };

    // 三月七6星被动：就这样，一直...
    // 在三月七提供的护盾保护下的我方目标，回合开始时，回复6点生命。
    struct March7thS6 : public Trigger {
        int heal = 6;

        March7thS6(BattleUnit& owner)
            : Trigger(owner, TriggerTime::TurnBegin) {}

        const char* name() const override {
            return "就这样，一直...";
        }

        std::string description() const override {
            return std::format("在三月七提供的护盾保护下的我方目标，回合开始时，回复{}点生命。", heal);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            if (!turn_begin.target.alive) return false;
            bool has_shield = false;
            for (auto& effect : turn_begin.target.effects) {
                if (effect->type() == EffectType::Shield) {
                    Shield& shield = dynamic_cast<Shield&>(*effect);
                    if (&shield.from == &target) {
                        has_shield = true;
                        break;
                    }
                }
            }
            return has_shield && turn_begin.target.side == target.side;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_begin = dynamic_cast<TurnBegin&>(procedure);
            HpUp{ turn_begin.battle, turn_begin.target,
                {}, heal * 10 }.invoke();
        }
    };

}
