#pragma once

#include "../utils/Utils.hpp"
#include "Battle.hpp"
#include "../cli/BattleCli.hpp"
#include "../utils/Trigger.hpp"
#include <vector>
#include <format>

namespace sr {

    // 用于战斗的流程基类
    struct Procedure {
        Battle& battle;         // 战场
        std::vector<Tag> tags;  // 标签

        // 构造函数
        Procedure(Battle& battle, std::vector<Tag> tags)
            : battle(battle), tags(tags) {}

        // 执行流程
        virtual void apply() = 0;

        // 调用流程
        void invoke() {
            if (battle.finished()) return;
            apply();
        }

        // 借用输出接口产生触发效果
        void on_trigger(Trigger& trigger, std::function<void()> callback) {
            battle.interface.print(std::format("【{}】触发了被动技能【{}】。", 
                trigger.target.colored_name(), trigger.name()));
            callback();
            battle.interface.print_raw(trigger.cli_text(battle));
        }

    };


}