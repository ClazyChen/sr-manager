#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/Break.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../effects/Slow.hpp"

namespace sr {

    // 瓦尔特被动1：时空扭曲
    // 普攻、战技或终结技命中减速目标时，附加威力65的虚数伤害。
    struct WeltP1 : public Trigger {
        int damage = 65; // 伤害

        WeltP1(BattleUnit& owner)
            : Trigger(owner, TriggerTime::AttackEnd) {}

        const char* name() const override {
            return "时空扭曲";
        }

        std::string description() const override {
            return std::format("普攻、战技或终结技命中减速目标时，附加威力{}的虚数伤害。", damage);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Attack& emit_skill = dynamic_cast<const Attack&>(procedure);
            // 要求是自己的技能
            return &emit_skill.attacker == &target && (
                has_tag(emit_skill.tags, Tag::NormalAttack) ||
                has_tag(emit_skill.tags, Tag::BattleSkill) ||
                has_tag(emit_skill.tags, Tag::UltimateSkill)
                ) && emit_skill.defender.effects.has(EffectType::Slow);
        }

        void on_trigger(Procedure& procedure) override {
            Attack& emit_skill = dynamic_cast<Attack&>(procedure);
            emit_skill.on_trigger(*this, [&] {
                Attack{
                        emit_skill.battle, target, emit_skill.defender,
                        { Tag::Extra, Tag::Single },
                        damage, Type::Imaginary
                }.invoke();
            });
        }
    };

    // 瓦尔特被动2：裁决
    // 攻击敌人时若目标处于击破状态，此次攻击中穿甲提高2点。
    struct WeltP2 : public Trigger {
        int pen_up = 2; // 穿甲提高

        WeltP2(BattleUnit& owner)
            : Trigger(owner, TriggerTime::AttackBegin) {}

        const char* name() const override {
            return "裁决";
        }

        std::string description() const override {
            return std::format("攻击敌人时若目标处于击破状态，此次攻击中穿甲提高{}点。", pen_up);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            return &attack.attacker == &target && attack.defender.toughness <= 0;
        }

        void on_trigger(Procedure& procedure) override {
            Attack& attack = dynamic_cast<Attack&>(procedure);
            attack.on_trigger(*this, [&] {
                attack.penetrate += pen_up;
            });
        }
    };

    // 瓦尔特被动3：画地为牢
    // 战斗开始时，所有敌人速度降低1点并推条2点（6命中・1回合）。
    struct WeltP3 : public Trigger {
        int speed_down = 1; // 速度降低
        int push = 2; // 推条

        WeltP3(BattleUnit& owner)
            : Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "画地为牢";
        }

        std::string description() const override {
            return std::format("战斗开始时，所有敌人速度降低{}点并推条{}点（6命中・1回合）。", speed_down, push);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                for (auto& unit : game_start.battle.opponents(target)) {
                    auto add_effect = AddEffect{
                        game_start.battle, target, *unit,
                        { Tag::AdditionalSkill, Tag::All },
                        std::make_unique<Slow>(1, target, speed_down),
                        6
                    };
                    add_effect.invoke();
                    if (add_effect.is_hit) {
                        game_start.battle.speed_bar.push(*unit, push);
                    }
                }
            });
        }
    };
}