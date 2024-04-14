#pragma once

#include "Trigger.hpp"
#include <vector>
#include <memory>

namespace sr {

    class Battle;

    struct TriggerList : public std::vector<std::unique_ptr<Trigger>> {

        Battle& battle;     // 战场

        // 构造函数
        TriggerList(Battle& battle) : battle(battle) {}

        // 添加触发器
        void add(std::unique_ptr<Trigger> trigger) {
            push_back(std::move(trigger));
        }

    };

}