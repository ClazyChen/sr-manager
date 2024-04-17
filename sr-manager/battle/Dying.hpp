#pragma once

#include "Procedure.hpp"
#include "Die.hpp"
#include "HpUp.hpp"
#include "../utils/effects/Unyielding.hpp"

namespace sr {

    // 实现濒死这一基本过程
    // 用于触发白露、杰帕德的复活技能
    struct Dying : public Procedure {
        BattleUnit& target;      // 目标

        // 构造函数
        Dying(Battle& battle, BattleUnit& target, std::vector<Tag> tags)
            : Procedure(battle, tags), target(target) {}

        // 执行濒死的效果
        void apply() override {
            if (!target.alive) return;
            // 触发不屈效果
            if (target.effects.has(EffectType::Unyielding)) {
                for (int i = 0; i < target.effects.size(); i++) {
                    if (target.effects[i]->type() == EffectType::Unyielding) {
                        Unyielding& unyielding = dynamic_cast<Unyielding&>(*target.effects[i]);
                        HpUp {
                            battle, target, tags, unyielding.heal - target.hp
                        }.invoke();
                        target.effects.erase(target.effects.begin() + i);
                        break;
                    }
                }
            }
            if (target.hp <= 0) {
                Die {
                    battle, target, tags
                }.invoke();
            }
        }
    };

}