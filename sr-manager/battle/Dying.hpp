#pragma once

#include "Procedure.hpp"
#include "Die.hpp"

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
            if (target.hp <= 0) {
                Die {
                    battle, target, tags
                }.invoke();
            }
        }
    };

}