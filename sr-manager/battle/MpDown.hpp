#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现消耗能量这一基本过程
    struct MpDown : public Procedure {
        BattleUnit& target;      // 目标
        int mp;                  // 能量值下降的数值

        // 构造函数
        MpDown(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int mp)
            : Procedure(battle, tags), target(target), mp(mp) {}

        // 执行消耗能量的效果
        void apply() override {
            if (!target.alive) return;
            target.mp -= mp;
            if (target.mp < 0) {
                target.mp = 0;
            }
        }

    };

}