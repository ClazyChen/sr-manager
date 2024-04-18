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
#include "../../../battle/HpDown.hpp"

namespace sr {

    // 阿兰A：如雷疾行
    // 对敌方单体造成威力100的雷伤害，削韧2。
    struct ArlanA : public Skill {
        int damage = 100;

        const char* name() const { return "如雷疾行"; }
        std::string description() const {
            return std::format(
                "对敌方单体造成威力{}的雷伤害，削韧2。",
                Painter::red(std::to_string(damage))
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Lightning
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack{ battle, user, target, tags(), damage, Type::Lightning }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness{ battle, user, target, tags(), 2, Type::Lightning }.invoke();
            // 获得 1 个战技点
            GetSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 阿兰E：禁锢解除
    // 消耗20点生命（不消耗战技点），对敌方单体造成威力240的雷伤害，削韧3。
    struct ArlanE : public Skill {
        int damage = 240;
        int cost_hp = 20;
        int bonus_damage = 0;

        const char* name() const { return "禁锢解除"; }
        std::string description() const {
            return std::format(
                "消耗{}点生命（不消耗战技点），对敌方单体造成威力{}的雷伤害，削韧3。{}",
                Painter::green(std::to_string(cost_hp)),
                Painter::red(std::to_string(damage)),
                bonus_damage > 0 ? std::format("生命值在50%以下时此技能威力提高{}。", bonus_damage) : ""
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Lightning
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 消耗 30 点生命（至少保留 1 点）
            HpDown{ battle, user, tags(), std::min(user.hp - 1, cost_hp * 10) }.invoke();
            // 对目标造成伤害
            int d = damage;
            if (bonus_damage > 0 && user.hp <= user.max_hp / 2) {
                d += bonus_damage;
            }
            Attack{ battle, user, target, tags(), d, Type::Lightning }.invoke();
            // 削减目标 3 点韧性
            ReduceToughness{ battle, user, target, tags(), 3, Type::Lightning }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 阿兰Q：狂者制裁
    // 消耗3点能量，对敌方扩散造成威力285（副目标150）的雷伤害，削韧4（副目标2）。
    struct ArlanQ : public Skill {
        int damage_main = 285;
        int damage_side = 150;
        int cost_mp = 3;

        const char* name() const { return "狂者制裁"; }
        std::string description() const {
            return std::format(
                "消耗{}点能量，对敌方扩散造成威力{}（副目标{}）的雷伤害，削韧4（副目标2）。",
                Painter::yellow(std::to_string(cost_mp)),
                Painter::red(std::to_string(damage_main)),
                Painter::red(std::to_string(damage_side))
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Spread };
        }

        auto _damage_side(BattleUnit& user) const {
            return user.star >= 6 && user.hp <= user.max_hp / 2 ? damage_main : damage_side;
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_spread(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage_main, Type::Lightning
                    });
                }, [&](BattleUnit& u) {
                    return ai::attack_expectation(Attack{
                        battle, user, u, tags(), _damage_side(user), Type::Lightning
                        });
                    });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 消耗 3 点能量
            MpDown{ battle, user, tags(), 3 }.invoke();
            // 对目标造成伤害
            Attack{ battle, user, target, tags(), damage_main, Type::Lightning }.invoke();
            ReduceToughness{ battle, user, target, tags(), 4, Type::Lightning }.invoke();
            for (auto& u : battle.opponents(user)) {
                if (u.get() == battle.left(target) || u.get() == battle.right(target)) {
                    Attack{ battle, user, *u, tags(), _damage_side(user), Type::Lightning}.invoke();
                    // 削减副目标韧性
                    ReduceToughness{ battle, user, *u, tags(), 2, Type::Lightning }.invoke();
                }
            }
        }
    };

}