#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现生命值下降这一基本过程
    struct HpUp : public Procedure {
        BattleUnit& target;    // 目标
        int heal;              // 生命值上升的数值

        // 构造函数
        HpUp(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int heal)
            : Procedure(battle, tags), target(target), heal(heal) {}

        // 执行生命值下降的效果
        void apply() override {
            if (!target.alive) return;
            target.hp += heal;
            if (target.hp > target.max_hp) {
                target.hp = target.max_hp;
            }
            battle.interface.print(std::format("【{}】生命值回复 {:0.1f} 点，剩余生命值 {:0.1f} 。"
                , target.colored_name(), heal / 10.0, target.hp / 10.0
            ));
        }

    };
}