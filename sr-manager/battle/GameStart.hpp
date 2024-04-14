#pragma once

#include "Procedure.hpp"

namespace sr {

    // 游戏开始流程
    struct GameStart : public Procedure {

        // 构造函数
        GameStart(Battle& battle)
            : Procedure(battle, { }) {}

        // 执行流程
        void apply() override {
            battle.invoke(TriggerTime::GameStart, *this);
        }

    };

}