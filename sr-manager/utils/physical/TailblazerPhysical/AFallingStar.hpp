#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/MpUp.hpp"

namespace sr {

    // 开拓者（物理）的被动：坠临万界的星芒（1星开启）
    struct AFallingStar : public Trigger {
        
        // 构造函数
        AFallingStar(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::Kill) {}

        // 名称
        const char* name() const override {
            return "坠临万界的星芒";
        }

        // 描述
        std::string description() const override {
            return "以终结技方式消灭敌人时，回复1点能量。";
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】回复1点能量。\n", target.colored_name());
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 const Attack& 类型
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            // 要求击杀的施加者是自己，且以终结技方式
            return &attack.attacker == &target && has_tag(attack.tags, Tag::UltimateSkill);
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Attack& 类型
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&]{
                // 回复 1 点能量
                MpUp { attack.battle, target, attack.tags, 1 }.invoke();
            });
        }

    };

}