#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 减速效果
    struct Slow : public Effect {
        
        BattleUnit& from;       // 施加者
        int damage;             // 威力

        // 构造函数
        Slow(int duration, BattleUnit& from, int damage)
            : Effect(duration), from(from), damage(damage) {}

        EffectType type() const override {
            return EffectType::Slow;
        }

        std::string name() const override {
            return "减速";
        }

        std::string description() const override {
            return std::format("速度降低{}点。（由【{}】附加）", damage, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            auto cost_before = unit.time_cost();
            // 降低目标速度
            unit.speed -= damage;
            auto cost_after = unit.time_cost();
            battle.speed_bar.move(unit, cost_before, cost_after);
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
        }
    };

}