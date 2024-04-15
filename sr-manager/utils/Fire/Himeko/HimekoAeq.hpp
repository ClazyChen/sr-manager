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

namespace sr {

    // 姬子A：武装调律
    struct HimekoA : public Skill {
        int damage = 100;

        const char* name() const { return "武装调律"; }
        std::string description() const { return std::format(
            "对敌方单体造成威力{}的火伤害，削韧2。"
            , Painter::red(std::to_string(damage))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack {
                    battle, user, u, tags(), damage, Type::Fire
                });
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack { battle, user, target, tags(), damage, Type::Fire }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness { battle, user, target, tags(), 2, Type::Fire }.invoke();
            // 获得 1 个战技点
            GetSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };
    
    // 姬子E：熔核爆裂
    struct HimekoE : public Skill {
        int damage_main = 200;
        int damage_side = 80;
        int damage_main_burn = 40;
        int damage_side_burn = 15;

        const char* name() const { return "熔核爆裂"; }
        std::string description() const { return std::format(
            "对敌方扩散造成威力{}（副目标{}）的火伤害，削韧2（副目标1）。此技能对于灼烧状态的目标威力提高{}（副目标{}）。"
            , Painter::red(std::to_string(damage_main))
            , Painter::red(std::to_string(damage_side))
            , Painter::red(std::to_string(damage_main_burn))
            , Painter::red(std::to_string(damage_side_burn))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Spread };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_spread(battle, user, [&](BattleUnit& u) {
                if (u.effects.has(EffectType::Burn)) {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage_main + damage_main_burn, Type::Fire
                    });
                } else {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage_main, Type::Fire
                    });
                }
            }, [&](BattleUnit& u) {
                if (u.effects.has(EffectType::Burn)) {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage_side + damage_side_burn, Type::Fire
                    });
                } else {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage_side, Type::Fire
                    });
                }
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对主目标造成伤害
            if (target.effects.has(EffectType::Burn)) {
                Attack { battle, user, target, tags(), damage_main + damage_main_burn, Type::Fire }.invoke();
            } else {
                Attack { battle, user, target, tags(), damage_main, Type::Fire }.invoke();
            }
            // 削减主目标韧性
            ReduceToughness { battle, user, target, tags(), 3, Type::Fire }.invoke();
            // 对副目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (u.get() == battle.left(target) || u.get() == battle.right(target)) {
                    if (u->effects.has(EffectType::Burn)) {
                        Attack { battle, user, *u, tags(), damage_side + damage_side_burn, Type::Fire }.invoke();
                    } else {
                        Attack { battle, user, *u, tags(), damage_side, Type::Fire }.invoke();
                    }
                    // 削减副目标韧性
                    ReduceToughness { battle, user, *u, tags(), 2, Type::Fire }.invoke();
                }
            }
            // 消耗 1 个战技点
            CostSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

    // 姬子Q：天坠之火
    struct HimekoQ : public Skill {

        int damage = 230;

        int extra_damage = 0;
        // 6星效果：造成两次随机单体伤害，削韧1

        // 群攻
        const char* name() const { return "天坠之火"; }
        std::string description() const { return std::format(
            "消耗3点能量，对敌方全体造成威力{}的火伤害，削韧2。{}"
            , Painter::red(std::to_string(damage))
            , (
                extra_damage > 0
                ? std::format("附加2次随机目标威力{}的火伤害，削韧1。", extra_damage)
                : ""
            )
        ); }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            // 全体攻击，随便选择一个目标即可
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    return *u;
                }
            }
            // 应该不会执行到这里
            return *battle.opponents(user).front();
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            MpDown { battle, user, tags(), 3 }.invoke();
            // 对目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (!u->alive) continue;
                Attack { battle, user, *u, tags(), damage, Type::Fire }.invoke();
                // 削减目标 2 点韧性
                ReduceToughness { battle, user, *u, tags(), 4, Type::Fire }.invoke();
            }
            if (extra_damage == 0) {
                return;
            }
            // 随机目标的额外伤害
            for (int i = 0; i < 2; i++) {
                // 如果游戏已经结束，不再继续
                if (battle.finished()) {
                    return;
                }
                // 随机选择目标
                auto& target = battle.random_target(battle.opponents(user));
                // 对目标造成伤害
                Attack { battle, user, *target, tags(), extra_damage, Type::Fire }.invoke();
                // 削减目标 1 点韧性
                ReduceToughness { battle, user, *target, tags(), 2, Type::Fire }.invoke();
            }
        }

    };

}