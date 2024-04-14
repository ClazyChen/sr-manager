#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现死亡这一基本过程
    struct Die : public Procedure {
        BattleUnit& target;      // 目标

        // 构造函数
        Die(Battle& battle, BattleUnit& target, std::vector<Tag> tags)
            : Procedure(battle, tags), target(target) {}

        // 执行死亡的效果
        void apply() override {
            if (!target.alive) return;
            // 执行死亡效果
            target.alive = false;
            target.mp = 0;
            target.toughness = 0;
            battle.interface.print(std::format("【{}】进入无法战斗状态。", target.colored_name()));
            battle.judge_success();
            // 从速度条中移除
            battle.speed_bar.remove(target);
        }
    };

}