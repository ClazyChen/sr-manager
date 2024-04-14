#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/GameStart.hpp"

namespace sr {

    // 开拓者（物理）的被动：不灭三振
    struct ImmortalThirdStrike : public Trigger {
        int max_hp_up = 15;        // 最大生命值提升

        // 构造函数
        ImmortalThirdStrike(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        // 名称
        const char* name() const override {
            return "不灭三振";
        }

        // 描述
        std::string description() const override {
            return std::format("战斗开始时，我方全体的最大生命值提升{}点。", Painter::cyan(std::to_string(max_hp_up)));
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            std::string temp = "";
            for (const auto& unit : battle.teammates(target)) {
                temp += std::format("【{}】的最大生命值提升{}点。\n", unit->colored_name(), max_hp_up);
            }
            return temp;
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&]{
                for (auto& unit : game_start.battle.teammates(target)) {
                    unit->max_hp += max_hp_up * 10;
                    unit->hp += max_hp_up * 10;
                }
            });
        }
    };

}