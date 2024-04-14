#pragma once

#include "Procedure.hpp"

namespace sr {

    // 消耗战技点数这一基本过程
    struct CostSkillPoint : public Procedure {
        BattleUnit& target;      // 目标
        int point;               // 消耗的技能点数

        // 构造函数
        CostSkillPoint(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int point)
            : Procedure(battle, tags), target(target), point(point) {}

        // 执行消耗技能点数的效果
        void apply() override {
            auto& p = battle.skill_points(target.side);
            p -= point;
        }
    };

}