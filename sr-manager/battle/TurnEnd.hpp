#pragma once

#include "Procedure.hpp"

namespace sr {

    // 回合结束流程
    struct TurnEnd : public Procedure {

        BattleUnit& target;      // 目标

        // 构造函数
        TurnEnd(Battle& battle, BattleUnit& target)
            : Procedure(battle, { }), target(target) {}

        // 执行流程
        void apply() override {
            if (!target.alive) return;
            battle.invoke(TriggerTime::TurnEnd, *this);
        }

    };

}