#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/TurnBegin.hpp"
#include "../../../battle/HpUp.hpp"

namespace sr {

    // 开拓者（物理）的被动：因缘假合的人身（2星）
    struct AnUnwillingHost : public Trigger {
        
        int hp_up = 5;  // 回复的生命值

        // 构造函数
        AnUnwillingHost(BattleUnit& target)
            : Trigger(target, TriggerTime::TurnBegin) {}

        // 名称
        const char* name() const override {
            return "因缘假合的人身";
        }

        // 描述
        std::string description() const override {
            return std::format("回合开始时，回复{}点生命值。", Painter::green(std::to_string(hp_up)));
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            const TurnBegin& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            return &turn_begin.target == &target;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            TurnBegin& turn_begin = dynamic_cast<TurnBegin&>(procedure);
            turn_begin.on_trigger(*this, [&]{
                // 回复生命值
                HpUp { turn_begin.battle, target, turn_begin.tags, hp_up * 10 }.invoke();
            });
        }

    };
}