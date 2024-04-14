#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 开拓者（物理）的终结技能：星尘王牌
    class StardustAce : public Skill {
    public:
        int damage_single = 335;
        int damage_spread_main = 290;
        int damage_spread_side = 150;

        const char* name() const { return "星尘王牌"; }
        std::string description() const { return std::format(
            "消耗3点能量，在两种模式中选择一种发动：（1）全胜·再见安打：对敌方单体造成威力{}的物理伤害，削韧6。（2）全胜·安息全垒打：对敌方扩散造成威力{}（副目标{}）的物理伤害，削韧4（副目标3）。"
            , Painter::red(std::to_string(damage_single))
            , Painter::red(std::to_string(damage_spread_main))
            , Painter::red(std::to_string(damage_spread_side))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill };
        }

    private:
        std::vector<Tag> tags_single() const {
            return { Tag::UltimateSkill, Tag::Single };
        }

        std::vector<Tag> tags_spread() const {
            return { Tag::UltimateSkill, Tag::Spread };
        }

        double single_expectation(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            return ai::attack_expectation(Attack {
                battle, user, target, tags_single(), damage_single, Type::Physical
            });
        }

        double spread_expectation_main(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            return ai::attack_expectation(Attack {
                battle, user, target, tags_spread(), damage_spread_main, Type::Physical
            });
        }

        double spread_expectation_side(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            return ai::attack_expectation(Attack {
                battle, user, target, tags_spread(), damage_spread_side, Type::Physical
            });
        }

    public:
        // 执行技能的方式
        // 1-单体攻击 2-扩散攻击
        int judge_invoke_type(Battle& battle, BattleUnit& user, BattleUnit& target_single, BattleUnit& target_spread) const {
            double score_single = single_expectation(battle, user, target_single);
            double score_spread = spread_expectation_main(battle, user, target_spread);
            for (auto& u : battle.opponents(user)) {
                if (u.get() == battle.left(target_spread) || u.get() == battle.right(target_spread)) {
                    score_spread += spread_expectation_side(battle, user, *u);
                }
            }
            if (score_single > score_spread) {
                return 1;
            } else {
                return 2;
            }
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            auto& target_single = target_ai_single(battle, user, [&](BattleUnit& u) {
                return single_expectation(battle, user, u);
            });
            auto& target_spread = target_ai_spread(battle, user, 
                [&](BattleUnit& u) { return spread_expectation_main(battle, user, u); },
                [&](BattleUnit& u) { return spread_expectation_side(battle, user, u); }
            );
            auto type = judge_invoke_type(battle, user, target_single, target_spread);
            if (type == 1) {
                return target_single;
            } else {
                return target_spread;
            }
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            auto invoke_type = judge_invoke_type(battle, user, target, target);
            if (invoke_type == 1) {
                // 全胜·再见安打
                MpDown { battle, user, tags_single(), 3 }.invoke();
                Attack { battle, user, target, tags_single(), damage_single, Type::Physical }.invoke();
                ReduceToughness { battle, user, target, tags_single(), 4, Type::Physical }.invoke();
            } else {
                // 全胜·安息全垒打
                MpDown { battle, user, tags_spread(), 3 }.invoke();
                Attack { battle, user, target, tags_spread(), damage_spread_main, Type::Physical }.invoke();
                ReduceToughness { battle, user, target, tags_spread(), 3, Type::Physical }.invoke();
                for (auto& u : battle.opponents(user)) {
                    if (u.get() == battle.left(target) || u.get() == battle.right(target)) {
                        Attack { battle, user, *u, tags_spread(), damage_spread_side, Type::Physical }.invoke();
                        ReduceToughness { battle, user, *u, tags_spread(), 2, Type::Physical }.invoke();
                    }
                }
            }
        }
    
    
    };


}