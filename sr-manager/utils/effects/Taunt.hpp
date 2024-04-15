#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"

namespace sr {
    
    // 嘲讽效果
    struct Taunt : public Effect {

        BattleUnit& from;     // 施加者
        
        // 构造函数
        Taunt(int duration, BattleUnit& from) : 
            Effect(duration), from(from) {}

        EffectType type() const override {
            return EffectType::Taunt;
        }

        EffectAttr attr() const override {
            return EffectAttr::Negative;
        }

        std::string name() const override {
            return std::format("受【{}】嘲讽", from.colored_name());
        }

        std::string description() const override {
            return std::format("无法攻击其他目标，只能攻击【{}】", from.colored_name());
        }

        void on_turn_end(Battle& battle, BattleUnit& unit) {
            duration--;
        }


    };

}