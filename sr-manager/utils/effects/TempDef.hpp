#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 临时防御提升效果
    struct TempDef : public Effect {

        BattleUnit& from;       // 施加者
        int damage;             // 威力

        // 构造函数
        TempDef(int duration, BattleUnit& from, int damage)
            : Effect(duration), from(from), damage(damage) {}

        EffectType type() const override {
            return EffectType::Else;
        }

        EffectAttr attr() const override {
            return EffectAttr::Positive;
        }

        std::string name() const override {
            return "防御提升";
        }

        std::string description() const override {
            return std::format("防御提升{}点。（由【{}】附加）", damage, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            unit.defense += damage;
        }

        void on_remove(Battle& battle, BattleUnit& unit) {
            unit.defense -= damage;
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
            // 恢复防御
            if (duration <= 0) {
                unit.defense -= damage;
            }
        }
    };

}