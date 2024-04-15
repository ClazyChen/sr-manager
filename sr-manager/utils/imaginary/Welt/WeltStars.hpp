#pragma once

#include "../../Utils.hpp"
#include "../../Trigger.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/AttackUpgrade.hpp"

namespace sr {

    // 瓦尔特1星被动：名的传承
    // 释放终结技后，获得普攻强效和战技强效50点（1回合）
    struct WeltS1 : public Trigger {
        int damage = 50;

        WeltS1(BattleUnit& owner)
            : Trigger(owner, TriggerTime::SkillEnd) {}
        
        const char* name() const override {
            return "名的传承";
        }

        std::string description() const override {
            return std::format("释放终结技后，获得普攻强效和战技强效{}点（1回合）。", damage);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            const EmitSkill& emit_skill = dynamic_cast<const EmitSkill&>(procedure);
            return &emit_skill.user == &target && has_tag(emit_skill.tags, Tag::UltimateSkill);
        }

        void on_trigger(Procedure& procedure) override {
            EmitSkill& emit_skill = dynamic_cast<EmitSkill&>(procedure);
            emit_skill.on_trigger(*this, [&] {
                AddEffect{
                    emit_skill.battle, target, target,
                    { },
                    std::make_unique<AttackUpgrade>(1, target, Tag::NormalAttack, damage),
                }.invoke();
                AddEffect{
                    emit_skill.battle, target, target,
                    { },
                    std::make_unique<AttackUpgrade>(1, target, Tag::BattleSkill, damage),
                }.invoke();
            });
        }
    };

}