#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 加速效果
    struct Fast : public Effect {
        
        BattleUnit& from;       // 施加者
        int speed;              // 速度

        // 构造函数
        Fast(int duration, BattleUnit& from, int speed)
            : Effect(duration), from(from), speed(speed) {}

        EffectType type() const override {
            return EffectType::Fast;
        }

        std::string name() const override {
            return "加速";
        }

        std::string description() const override {
            return std::format("速度提升{}点。（由【{}】附加）", speed, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            auto cost_before = unit.time_cost();
            // 增加目标速度
            unit.speed += speed;
            auto cost_after = unit.time_cost();
            battle.speed_bar.move(unit, cost_before, cost_after);
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
        }
    };

}