#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 护盾效果
    struct Shield : public Effect {

        BattleUnit& from;  // 施加者
        int shield;  // 护盾值

        // 构造函数
        Shield(int duration, BattleUnit& from, int shield) 
            : Effect(duration), from(from), shield(shield) {}

        EffectType type() const override {
            return EffectType::Shield;
        }

        EffectAttr attr() const override {
            return EffectAttr::Positive;
        }

        std::string name() const override {
            return "护盾";
        }

        std::string description() const override {
            return std::format("护盾 {} 点。（由【{}】附加）", shield, from.colored_name());
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) override {
            duration--;
        }
    };

    int shield_value(const BattleUnit& unit) {
        int shield = 0;
        for (auto& effect : unit.effects) {
            if (effect->type() == EffectType::Shield) {
                auto& shield_effect = dynamic_cast<Shield&>(*effect);
                shield = std::max(shield, shield_effect.shield);
            }
        }
        return shield;
    }

}