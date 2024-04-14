#pragma once

#include "Procedure.hpp"
#include "Break.hpp"

namespace sr {

    // 实现削减韧性这一基本过程
    struct ReduceToughness : public Procedure {
        BattleUnit& from;        // 施加者
        BattleUnit& target;      // 目标
        int toughness;           // 韧性值下降的数值
        Type type;               // 伤害类型

        // 构造函数
        ReduceToughness(Battle& battle, BattleUnit& from, BattleUnit& target, std::vector<Tag> tags, int toughness, Type type)
            : Procedure(battle, tags), from(from), target(target), toughness(toughness), type(type) {}

        // 执行削减韧性的效果
        void apply() override {
            if (!target.alive) return;
            auto toughness_before = target.toughness;
            target.toughness -= toughness;
            if (target.toughness <= 0) {
                target.toughness = 0;
                // 触发击破效果！
                if (toughness_before > 0) {
                    Break { battle, from, target, tags, type }.invoke();
                }
            }
        }

    };

}