#pragma once

#include "../../Utils.hpp"
#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/Break.hpp"

namespace sr {

    // 姬子2星被动：邂逅
    // 攻击敌人时若目标生命值一半以下，此次攻击中穿甲提高1点
    struct HimekoS2 : public Trigger {

        int pen_up = 1; // 穿甲提高的数值

        // 构造函数
        HimekoS2(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::AttackBegin) {}
        
        // 名称
        const char* name() const override {
            return "邂逅";
        }

        // 描述
        std::string description() const override {
            return std::format("攻击敌人时若目标生命值一半以下，此次攻击中穿甲提高{}点。",
                Painter::cyan(std::to_string(pen_up))
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
            // 要求攻击的施加者是自己，且目标生命值一半以下
            return &attack.attacker == &target && attack.defender.hp <= attack.defender.max_hp / 2;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Attack& 类型
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&]{ 
                // 穿甲提高 1 点
                attack.penetrate += pen_up;
            });
        }

    };

    // 姬子4星被动：投入
    // 击破弱点时，获得1点能量
    struct HimekoS4 : public Trigger {

        // 构造函数
        HimekoS4(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::Break) {}
        
        // 名称
        const char* name() const override {
            return "投入";
        }

        // 描述
        std::string description() const override {
            return "击破弱点时，回复1点能量。";
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】回复1点能量。\n", target.colored_name());
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 const Break& 类型
            const Break& break_ = dynamic_cast<const Break&>(procedure);
            // 要求击破的施加者是自己
            return &break_.from == &target; //&& has_tag(break_.tags, Tag::BattleSkill);
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 Break& 类型
            Break& break_ = dynamic_cast<Break&>(procedure);
            break_.on_trigger(*this, [&]{
                // 获得 1 点能量
                MpUp { break_.battle, target, {}, 1 }.invoke();
            });
        }

    };


}