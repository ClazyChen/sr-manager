#pragma once

#include "Procedure.hpp"
#include "Dying.hpp"

namespace sr {

    // 实现生命值下降这一基本过程
    struct HpDown : public Procedure {
        BattleUnit& target;      // 目标
        int damage;              // 生命值下降的数值

        // 构造函数
        HpDown(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int damage)
            : Procedure(battle, tags), target(target), damage(damage) {}

        // 执行生命值下降的效果
        void apply() override {
            if (!target.alive) return;
            target.hp -= damage;
            battle.interface.print(std::format("【{}】生命值下降 {:0.1f} 点，剩余生命值 {:0.1f} 。"
                , target.colored_name(), damage / 10.0, target.hp / 10.0
            ));
            if (target.hp < 0) {
                // 触发濒死效果
                Dying {
                    battle, target, tags
                }.invoke();
            }
        }

    };
}