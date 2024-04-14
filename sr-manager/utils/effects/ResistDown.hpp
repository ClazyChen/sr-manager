#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {

    // 元素抗性降低效果
    struct ResistDown : public Effect {
        
        BattleUnit& from;       // 施加者
        Type element_type;              // 元素类型
        int damage;             // 威力

        // 构造函数
        ResistDown(int duration, BattleUnit& from, Type element_type, int damage)
            : Effect(duration), from(from), element_type(element_type), damage(damage) {}

        EffectType type() const override {
            return EffectType::ResistDown;
        }

        std::string name() const override {
            return std::format("{}抗性降低", type_name(element_type));
        }

        std::string description() const override {
            return std::format("{}抗性降低{}点。（由【{}】附加）", type_name(element_type), damage, from.colored_name());
        }

        void on_apply(Battle& battle, BattleUnit& unit) {
            
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // 持续时间减少
            duration--;
        }
    };

}