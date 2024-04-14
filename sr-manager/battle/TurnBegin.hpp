#pragma once

#include "Procedure.hpp"

namespace sr {

    // 回合开始流程
    struct TurnBegin : public Procedure {

        BattleUnit& target;      // 目标

        // 构造函数
        TurnBegin(Battle& battle, BattleUnit& target)
            : Procedure(battle, { }), target(target) {}

        // 执行流程
        void apply() override {
            if (!target.alive) return;
            battle.invoke(TriggerTime::TurnBegin, *this);
        }

    };

}