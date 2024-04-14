#pragma once

#include "../../Utils.hpp"
#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"

namespace sr {

    // 1星被动
    struct DanHengS1 : public Trigger {

        int critical_up = 1; // 暴击提高的数值

        // 构造函数
        DanHengS1(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::AttackBegin) {}
        
        // 名称
        const char* name() const override {
            return "穷高极天，亢盈难久";
        }

        // 描述
        std::string description() const override {
            return std::format("攻击敌人时若目标生命值一半以上，此次攻击中暴击提高{}点。",
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
            // 要求攻击的施加者是自己，且目标生命值一半以上
            return &attack.attacker == &target && attack.defender.hp >= attack.defender.max_hp / 2;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Attack& 类型
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&]{ 
                // 暴击提高 1 点
                attack.critical_rate += critical_up;
            });
        }

    };

    // 4星被动
    //  [星4], [奋迅三昧，如日空居], [终结技消灭敌人时立刻插入一个额外的回合。],
    struct DanHengS4 : public Trigger {

        // 构造函数
        DanHengS4(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::Kill) {}
        
        // 名称
        const char* name() const override {
            return "奋迅三昧，如日空居";
        }

        // 描述
        std::string description() const override {
            return "以终结技方式消灭敌人时，插入一个额外的回合。";
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】插入一个额外的回合。\n", target.colored_name());
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
                // 插入一个额外的回合
                attack.battle.speed_bar.insert_extra(target);
            });
        }

    };

}