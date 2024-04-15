#pragma once

#include "../Utils.hpp"
#include "../../battle/Attack.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 灼烧效果
    struct Burn : public Effect {

        BattleUnit& from;        // 施加者
        int damage;             // 威力

        // 构造函数
        Burn(int duration, BattleUnit& from, int damage)
            : Effect(duration), from(from), damage(damage) {}

        EffectType type() const override {
            return EffectType::Burn;
        }

        EffectAttr attr() const override {
            return EffectAttr::Negative;
        }

        std::string name() const override {
            return "灼烧";
        }

        std::string description() const override {
            return std::format("回合开始时，受到 {} 威力的火伤害。（由【{}】附加）", damage, from.colored_name());
        }

        void on_turn_begin(Battle& battle, BattleUnit& unit) {
            // 对目标造成伤害
            Attack {
                battle, from, unit, 
                { Tag::DamageOverTime, Tag::Burn },
                damage, Type::Fire
            }.invoke();
            // 持续时间减少
            duration--;
        }

    };

}