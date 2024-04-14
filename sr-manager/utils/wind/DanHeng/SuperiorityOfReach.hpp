#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../effects/TempPen.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../battle/TurnBegin.hpp"

namespace sr {

    // 丹恒的被动：寸长寸强
    struct SuperiorityOfReach : public Trigger {
        
        int penetration = 2;    // 穿甲值
        int cooldown = 2;       // 冷却时间

        // 构造函数
        SuperiorityOfReach(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::SkillBegin) {
                if (owner.star >= 2) {
                    cooldown = 1;
                }
            }

        // 名称
        const char* name() const override {
            return "寸长寸强";
        }

        // 描述
        std::string description() const override {
            return std::format("成为我方技能释放目标时，获得{}点穿甲（1回合），冷却{}回合。", penetration, cooldown);
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return "";
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 EmitSkill& 类型
            const EmitSkill& emit_skill = dynamic_cast<const EmitSkill&>(procedure);
            // 要求目标是自己，冷却时间为 0，且是我方技能释放目标
            return &emit_skill.target == &target && cooldown == 0 && emit_skill.user.side == target.side;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            // 强制转换为 EmitSkill& 类型
            EmitSkill& emit_skill = dynamic_cast<EmitSkill&>(procedure);
            emit_skill.on_trigger(*this, [&]{
                // 施加临时穿甲提升效果
                AddEffect { 
                    emit_skill.battle, target, target, emit_skill.tags,
                    std::make_unique<TempPen>(1, target, penetration)
                }.invoke();
                // 设置冷却时间
                cooldown = 2;
            });
        }

    };

    // 用来控制冷却时间的辅助类
    struct SuperiorityOfReachCooldown : public Trigger {
        SuperiorityOfReach& reach;

        // 构造函数
        SuperiorityOfReachCooldown(SuperiorityOfReach& reach) : 
            Trigger(reach.target, TriggerTime::TurnBegin), reach(reach) {}

        // 自己的回合开始时触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 TurnBegin& 类型
            const TurnBegin& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            // 要求是自己的回合
            return &turn_begin.target == &target;
        }

        // 触发效果（不显示）
        void on_trigger(Procedure& procedure) override {
            reach.cooldown = std::max(0, reach.cooldown - 1);
        }


    };

}