#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现移除一个负面效果的过程
    struct RemoveDebuff : public Procedure {

        BattleUnit& from;   // 施加者
        BattleUnit& target; // 目标
        
        // 构造函数
        RemoveDebuff(Battle& battle, BattleUnit& from, BattleUnit& target, std::vector<Tag> tags)
            : Procedure(battle, tags), from(from), target(target) {}

        // 执行移除负面效果的过程
        // 移除同种的全部负面效果
        void apply() override {
            if (!target.alive) return;
            EffectType type = EffectType::Else;
            for (int i = 0; i < target.effects.size(); i++) {
                if (target.effects[i]->attr() == EffectAttr::Negative) {
                    if (type == EffectType::Else) {
                        type = target.effects[i]->type();
                    }
                    // 如果不是同种负面效果，跳过
                    if (type != target.effects[i]->type()) {
                        continue;
                    }
                    // 移除负面效果
                    target.effects[i]->on_remove(battle, target);
                    target.effects.erase(target.effects.begin() + i);
                    i--;
                }
            }
            if (type == EffectType::Else) {
                return;
            }
            // 通知
            battle.interface.print(std::format("【{}】移除了【{}】的负面效果 {}",
                from.colored_name(), target.colored_name(), type_name(type)));
        }

    };

}