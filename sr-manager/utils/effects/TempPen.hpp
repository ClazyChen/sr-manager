#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 临时穿甲提升效果
    struct TempPen : public Effect {
        
        BattleUnit& from;       // 施加者
        int damage;             // 威力

        // 构造函数
        TempPen(int duration, BattleUnit& from, int damage)
            : Effect(duration), from(from), damage(damage) {}

        EffectType type() const override {
            return EffectType::Else;
        }

        std::string name() const override {
            return "穿甲提升";
        }

        std::string description() const override {
            return std::format("穿甲提升{}点。（由【{}】附加）", damage, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            unit.penetration += damage;
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
        }
    };

}