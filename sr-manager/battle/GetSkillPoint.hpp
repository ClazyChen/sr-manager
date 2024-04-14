#pragma once

#include "Procedure.hpp"

namespace sr {

    // 获得战技点数这一基本过程
    struct GetSkillPoint : public Procedure {
        BattleUnit& target;      // 目标
        int point;               // 获得的技能点数

        // 构造函数
        GetSkillPoint(Battle& battle, BattleUnit& target, std::vector<Tag> tags, int point)
            : Procedure(battle, tags), target(target), point(point) {}

        // 执行获得技能点数的效果
        void apply() override {
            auto& p = battle.skill_points(target.side);
            p += point;
            if (p > Battle::MAX_SKILL_POINTS) {
                p = Battle::MAX_SKILL_POINTS;
            }
        }
    };

}