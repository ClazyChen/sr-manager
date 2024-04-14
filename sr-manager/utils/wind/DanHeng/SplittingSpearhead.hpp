#pragma once

#include "../../Trigger.hpp"
#include "../../effects/TempAtk.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../../battle/AddEffect.hpp"

namespace sr {

    // 丹恒的被动：破敌锋芒
    struct SplittingSpearhead : public Trigger {
        int attack_up = 4;
        int attack_up_duration = 3;

        // 构造函数
        SplittingSpearhead(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        // 名称
        const char* name() const override {
            return "破敌锋芒";
        }

        // 描述
        std::string description() const override {
            return std::format("战斗开始时，攻击提升{}点（{}回合）。"
                , Painter::cyan(std::to_string(attack_up))
                , attack_up_duration);
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】的攻击提升{}点（{}回合）。\n"
                , target.colored_name()
                , attack_up
                , attack_up_duration);
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&]{
                AddEffect {
                    game_start.battle, target, target, {}, 
                    std::make_unique<TempAtk>(attack_up_duration, target, attack_up)
                }.invoke();
            });
        }
    };

}