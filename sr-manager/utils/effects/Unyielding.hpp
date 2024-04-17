#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 不屈效果
    struct Unyielding : public Effect {

        BattleUnit& from;       // 施加者
        int heal;               // 治疗量

        // 构造函数
        Unyielding(int duration, BattleUnit& from, int heal) : Effect(duration), from(from), heal(heal) {}

        EffectType type() const override {
            return EffectType::Unyielding;
        }

        EffectAttr attr() const override {
            return EffectAttr::Positive;
        }

        std::string name() const override {
            return "不屈";
        }

        std::string description() const override {
            return std::format("即将进入无法战斗状态时，生命回复 {} 点。（由【{}】施加）", heal, from.colored_name());
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            duration--;
        }
    };
}