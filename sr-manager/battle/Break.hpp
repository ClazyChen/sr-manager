#pragma once

#include "Procedure.hpp"
#include "Attack.hpp"
#include "AddEffect.hpp"
#include "../utils/effects/Bleed.hpp"
#include "../utils/effects/WindShear.hpp"

namespace sr {

    // 击破效果
    struct Break : public Procedure {

        BattleUnit& from;       // 施加者
        BattleUnit& target;     // 目标
        Type type;              // 伤害类型

        // 构造函数
        Break(Battle& battle, BattleUnit& from, BattleUnit& target, std::vector<Tag> tags, Type type)
            : Procedure(battle, tags), from(from), target(target), type(type) {
                tags.push_back(Tag::Break); // 添加击破标签
            }

        // 执行击破的效果
        void apply() override {
            if (!target.alive) return;
            // 输出击破信息
            battle.interface.print(std::format("【{}】将【{}】韧性击破！！", from.colored_name(), target.colored_name()));
            // 根据伤害类型进行不同的处理
            switch (type) {
                case Type::Physical:
                    // 物理击破：造成威力200的物理伤害，附加威力100的裂伤状态（9命中·3回合）
                    Attack { battle, from, target, tags, 200, Type::Physical }.invoke();
                    AddEffect { battle, from, target, tags, std::make_unique<Bleed>(3, from, 100), 9 }.invoke();
                    break;
                case Type::Wind:
                    // 风击破：造成威力150的风伤害，附加威力100的风化状态（9命中·2回合）
                    Attack { battle, from, target, tags, 150, Type::Wind }.invoke();
                    AddEffect { battle, from, target, tags, std::make_unique<WindShear>(2, from, 100), 9 }.invoke();
                    break;
            }
            // 触发击破时机
            battle.invoke(TriggerTime::Break, *this);
        }

    };

}