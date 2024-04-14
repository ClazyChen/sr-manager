#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"

namespace sr {

    // 开拓者（物理）的被动：凝眸毁灭的瞬间（4星）
    struct ADestructingGlance : public Trigger {

        int critical_up = 2; // 暴击提高的数值
        
        // 构造函数
        ADestructingGlance(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::AttackBegin) {}

        // 名称
        const char* name() const override {
            return "凝眸毁灭的瞬间";
        }

        // 描述
        std::string description() const override {
            return std::format("攻击敌人时若目标处于击破状态，此次攻击中暴击提高{}点。",
                Painter::cyan(std::to_string(critical_up))
            );
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 const Attack& 类型
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            // 要求攻击的施加者是自己，且目标处于击破状态
            return &attack.attacker == &target && attack.defender.toughness <= 0;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Attack& 类型
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&]{ 
                // 暴击提高 2 点
                attack.critical_rate += critical_up;
            });
        }

    };

}