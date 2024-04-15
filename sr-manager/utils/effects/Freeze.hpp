#pragma once

#include "../Utils.hpp"
#include "../../battle/Attack.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 冻结效果
    struct Freeze : public Effect {

        BattleUnit& from;     // 施加者
        int damage;           // 威力

        // 构造函数
        Freeze(int duration, BattleUnit& from, int damage)
            : Effect(duration), from(from), damage(damage) {}

        EffectType type() const override { 
            return EffectType::Freeze; 
        }

        EffectAttr attr() const override {
            return EffectAttr::Negative;
        }

        std::string name() const override {
            return "冻结";
        }

        std::string description() const override {
            return std::format("无法行动，解除时受到 {} 威力的冰伤害。（由【{}】附加）"
                , damage, from.colored_name());
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
            // 如果持续时间为 0，解除效果
            if (duration == 0) {
                Attack{
                    battle, from, unit, 
                    { Tag::Freeze }, damage, Type::Ice
                }.invoke();
            }
        }

    };

}