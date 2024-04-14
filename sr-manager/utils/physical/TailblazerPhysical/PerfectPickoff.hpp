#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Break.hpp"

namespace sr {

    // 开拓者（物理）的被动：牵制盗垒
    struct PerfectPickoff : public Trigger {
        int attack_up = 2;      // 攻击力上升的数值
        int defense_up = 1;     // 防御力上升的数值
        int limit = 2;          // 触发次数上限
        int count = 0;          // 触发次数

        // 构造函数
        PerfectPickoff(BattleUnit& target)
            : Trigger(target, TriggerTime::Break) {}

        // 名称
        const char* name() const override {
            return "牵制盗垒";
        }

        // 描述
        std::string description() const override {
            return std::format("击破敌人时，自身攻击提高{}点，防御提高{}点。此效果每局游戏只能触发{}次。"
            , Painter::cyan(std::to_string(attack_up))
            , Painter::cyan(std::to_string(defense_up))
            , limit
            );
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】的攻击提高{}点，防御提高{}点。（已用次数：{}）\n"
                , target.colored_name()
                , attack_up, defense_up
                , (
                    limit == count ?
                    Painter::red(std::format("{}/{}", count, limit)) :
                    Painter::green(std::format("{}/{}", count, limit))
                )
            );
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 const Break& 类型
            const Break& break_ = dynamic_cast<const Break&>(procedure);
            // 要求击破的施加者是自己，且触发次数未达上限
            return &break_.from == &target && count < limit;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Break& 类型
            Break& break_ = dynamic_cast<Break&>(procedure);
            break_.on_trigger(*this, [&]{
                // 攻击力上升
                target.attack += attack_up;
                // 防御力上升
                target.defense += defense_up;
                // 触发次数增加
                count++;
            });
        }

    };

}