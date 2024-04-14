#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现回复能量这一基本过程
    struct MpUp : public Procedure {
        BattleUnit& target;      // 目标
        int mp;                  // 能量值上升的数值

        // 构造函数
        MpUp(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int mp)
            : Procedure(battle, tags), target(target), mp(mp) {}

        // 执行回复能量的效果
        void apply() override {
            if (!target.alive) return;
            target.mp += mp;
            if (target.mp > target.max_mp) {
                target.mp = target.max_mp;
            }
        }

    };

}