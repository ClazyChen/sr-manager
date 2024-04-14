#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 开拓者（物理）的战技：安息全垒打
    class RipHomeRun : public Skill {
    public:
        int damage_main = 150;
        int damage_side = 120;

        const char* name() const { return "安息全垒打"; }
        std::string description() const { return std::format(
            "对敌方扩散造成威力{}（副目标{}）的物理伤害，削韧2（副目标1）。"
            , Painter::red(std::to_string(damage_main))
            , Painter::red(std::to_string(damage_side))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Spread };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_spread(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack {
                    battle, user, u, tags(), damage_main, Type::Physical
                });
            }, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack {
                    battle, user, u, tags(), damage_side, Type::Physical
                });
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对主目标造成伤害
            Attack { battle, user, target, tags(), damage_main, Type::Physical }.invoke();
            // 削减主目标 2 点韧性
            ReduceToughness { battle, user, target, tags(), 2, Type::Physical }.invoke();
            // 对副目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (u.get() == battle.left(target) || u.get() == battle.right(target)) {
                    Attack { battle, user, *u, tags(), damage_side, Type::Physical }.invoke();
                    // 削减副目标 1 点韧性
                    ReduceToughness { battle, user, *u, tags(), 1, Type::Physical }.invoke();
                }
            }
            // 消耗 1 个战技点
            CostSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

}