#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../../battle/RemoveDebuff.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/Unyielding.hpp"

namespace sr {

    // 阿兰被动1：至痛至怒
    // 根据已损失生命值百分比获得威力加成，至多增加100威力。
    struct ArlanP1 : public Trigger {
        int damage_limit = 100;

        ArlanP1(BattleUnit& owner) :
            Trigger(owner, TriggerTime::AttackBegin) {}

        const char* name() const override {
            return "至痛至怒";
        }

        std::string description() const override {
            return std::format("根据已损失生命值百分比获得威力加成，至多增加{}威力。",
                damage_limit);
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            return &attack.attacker == &target;
        }

        void on_trigger(Procedure& procedure) override {
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&] {
                int damage_bonus = (target.max_hp - target.hp) * damage_limit / target.max_hp;
                attack.damage += damage_bonus;
                });
        }
    };

    // 阿兰被动2：急速收割
    // 进入战斗时，对敌方全体削韧4。
    struct ArlanP2 : public Trigger {
        ArlanP2(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "急速收割";
        }

        std::string description() const override {
            return "进入战斗时，对敌方全体削韧4。";
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                for (auto& unit : game_start.battle.opponents(target)) {
                    ReduceToughness{ game_start.battle, target, *unit, 
                        { }, 4, Type::Lightning }.invoke();
                }
                });
        }
    };

    // 阿兰2星被动：除制去缚
    // 施放战技或终结技时，解除1个负面效果。
    struct ArlanS2 : public Trigger {
        ArlanS2(BattleUnit& owner) :
            Trigger(owner, TriggerTime::SkillEnd) {}

        const char* name() const override {
            return "除制去缚";
        }

        std::string description() const override {
            return "施放战技或终结技时，解除1个负面效果。";
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            const EmitSkill& emit_skill = dynamic_cast<const EmitSkill&>(procedure);
            return &emit_skill.user == &target && (
                has_tag(emit_skill.tags, Tag::BattleSkill) ||
                has_tag(emit_skill.tags, Tag::UltimateSkill));
        }

        void on_trigger(Procedure& procedure) override {
            EmitSkill& emit_skill = dynamic_cast<EmitSkill&>(procedure);
            emit_skill.on_trigger(*this, [&] {
                RemoveDebuff{ emit_skill.battle, target, target, {} }.invoke();
                });
        }
    };

    // 阿兰4星被动：绝处反击
    // 进入战斗时，获得不屈状态40（2回合）。
    struct ArlanS4 : public Trigger {
        int heal = 40;

        ArlanS4(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "绝处反击";
        }

        std::string description() const override {
            return std::format("进入战斗时，获得不屈状态{}（2回合）。", heal);
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                AddEffect{ game_start.battle, target, target, {},
                std::make_unique<Unyielding>(2, target, heal * 10) }.invoke();
                });
        }
    };


}