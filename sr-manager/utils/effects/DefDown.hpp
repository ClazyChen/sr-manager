#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 防御降低效果
    struct DefDown : public Effect {

        BattleUnit& from;        // 施加者
        int value;              // 提升值

        // 构造函数
        DefDown(int duration, BattleUnit& from, int value)
            : Effect(duration), from(from), value(value) {}

        EffectType type() const override {
            return EffectType::Else;
        }

        EffectAttr attr() const override {
            return EffectAttr::Negative;
        }

        std::string name() const override {
            return "防御下降";
        }

        std::string description() const override {
            return std::format("防御下降 {} 点。（由【{}】附加）", value, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            // 降低防御
            unit.defense -= value;
        }

        void on_remove(Battle& battle, BattleUnit& unit) {
            // 恢复防御
            unit.defense += value;
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
            // 恢复防御
            if (duration <= 0) {
                unit.defense += value;
            }
        }

    };

}