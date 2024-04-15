#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 攻击强效
    struct AttackUpgrade : public Effect {
        
        BattleUnit& from;       // 施加者
        Tag attack_tag;         // 攻击标签
        int damage;             // 威力

        // 构造函数
        AttackUpgrade(int duration, BattleUnit& from, Tag attack_tag, int damage)
            : Effect(duration), from(from), attack_tag(attack_tag), damage(damage) {}
        
        EffectType type() const override {
            return EffectType::AttackUpgrade;
        }

        std::string name() const override {
            return std::format("{}强效", tag_name(attack_tag));
        }

        std::string description() const override {
            return std::format("{}威力提升{}点。（由【{}】附加）", 
                tag_name(attack_tag),
                damage, from.colored_name());
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            duration--;
        }
    };
}