#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"
#include "../../effects/Freeze.hpp"
#include "../../effects/TempAtk.hpp"
#include "../../../battle/AddEffect.hpp"

namespace sr {

    // 黑塔A：看什么看
    // 对敌方单体造成威力100的冰伤害，削韧2。
    struct HertaA : public Skill {
        int damage = 100;
        int damage_bonus = 0;

        const char* name() const { return "看什么看"; }
        std::string description() const {
            return std::format(
                "对敌方单体造成威力{}的冰伤害，削韧2。{}",
                Painter::red(std::to_string(damage)),
                damage_bonus > 0 ?
                std::format("此技能对于生命值50%以下的目标威力提高{}。", Painter::red(std::to_string(damage_bonus))) :
                    ""
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Ice
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            int d = damage;
            if (damage_bonus > 0 && target.hp <= target.max_hp / 2) {
                d += damage_bonus;
            }
            Attack{ battle, user, target, tags(), d, Type::Ice }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness{ battle, user, target, tags(), 2, Type::Ice }.invoke();
            // 获得 1 个战技点
            GetSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 黑塔E：一锤子买卖
    // 对敌方全体造成威力100的冰伤害，削韧3。此技能对于生命值50%以上的目标威力提高45。
    struct HertaE : public Skill {
        int damage = 100;
        int bonus_damage = 45;

        const char* name() const { return "一锤子买卖"; }
        std::string description() const {
            return std::format(
                "对敌方全体造成威力{}的冰伤害，削韧3。此技能对于生命值50%以上的目标威力提高{}。",
                Painter::red(std::to_string(damage)),
                Painter::red(std::to_string(bonus_damage))
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    int d = damage;
                    if (u->hp >= u->max_hp / 2) {
                        d += bonus_damage;
                    }
                    // 对目标造成伤害
                    Attack{ battle, user, *u, tags(), d, Type::Ice }.invoke();
                    // 削减目标 3 点韧性
                    ReduceToughness{ battle, user, *u, tags(), 3, Type::Ice }.invoke();
                }
            }
            // 消耗 1 个战技点
            CostSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 黑塔Q：是魔法，我加了魔法
    // 消耗3点能量，对敌方全体造成威力200的冰伤害，削韧4。此技能对于冻结状态的目标威力提高40。
    struct HertaQ : public Skill {
        int damage = 200;
        int bonus_damage = 40;
        int attack_up = 0;
        int attack_up_duration = 1;

        const char* name() const { return "是魔法，我加了魔法"; }
        std::string description() const {
            return std::format(
                "消耗3点能量，对敌方全体造成威力{}的冰伤害，削韧4。此技能对于冻结状态的目标威力提高{}。{}",
                Painter::red(std::to_string(damage)),
                Painter::red(std::to_string(bonus_damage)),
                attack_up > 0 ? std::format("攻击提高{}点", attack_up) : ""
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            MpDown{ battle, user, tags(), 3 }.invoke();
            // 对目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    int d = damage;
                    if (u->effects.has(EffectType::Freeze)) {
                        d += bonus_damage;
                    }
                    // 对目标造成伤害
                    Attack{ battle, user, *u, tags(), d, Type::Ice }.invoke();
                    // 削减目标 4 点韧性
                    ReduceToughness{ battle, user, *u, tags(), 4, Type::Ice }.invoke();
                }
            }
            if (attack_up > 0) {
                AddEffect{
                    battle, user, user, tags(),
                    std::make_unique<TempAtk>(attack_up_duration, user, attack_up)
                }.invoke();
            }
        }
    };
}