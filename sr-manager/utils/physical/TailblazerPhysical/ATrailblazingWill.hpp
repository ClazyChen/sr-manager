#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "PerfectPickoff.hpp"

namespace sr {

    // 开拓者（物理）的被动：拓宇行天的意志（6星）
    struct ATrailblazingWill : public PerfectPickoff {
        
        // 构造函数
        ATrailblazingWill(BattleUnit& owner) : 
            PerfectPickoff(owner) {
                // 改变设置触发时机
                time = TriggerTime::Kill;
                limit = std::numeric_limits<int>::max(); // 不限制次数
            }

        // 名称
        const char* name() const override {
            return "拓宇行天的意志";
        }

        // 描述
        std::string description() const override {
            return "消灭敌人时也可以触发【牵制盗垒】的效果，不计入【牵制盗垒】的次数限制。";
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】的攻击提高{}点，防御提高{}点。\n"
                , target.colored_name()
                , attack_up, defense_up
            );
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 const Attack& 类型
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            // 要求击杀的施加者是自己
            return &attack.attacker == &target;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Attack& 类型
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&]{
                // 攻击力上升
                target.attack += attack_up;
                // 防御力上升
                target.defense += defense_up;
            });
        }

    };

}