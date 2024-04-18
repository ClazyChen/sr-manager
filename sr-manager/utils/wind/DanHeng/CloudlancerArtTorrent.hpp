#pragma once

#include "../../effects/Slow.hpp"
#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 丹恒的战技：云骑枪术·疾雨
    class CloudlancerArtTorrent : public Skill {
    public:
        int damage = 260;
        int slow = 1;
        int slow_hit_rate = 3;
        int slow_duration = 2;

        const char* name() const { return "云骑枪术·疾雨"; }
        std::string description() const { return std::format(
            "对敌方单体造成威力{}的风伤害，削韧3。降低目标{}点速度（{}命中·{}回合）。"
            , Painter::red(std::to_string(damage))
            , slow
            , slow_hit_rate
            , slow_duration
        ); }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack {
                    battle, user, u, tags(), damage, Type::Wind
                });
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack { battle, user, target, tags(), damage, Type::Wind }.invoke();
            // 削减目标 4 点韧性
            ReduceToughness { battle, user, target, tags(), 3, Type::Wind }.invoke();
            // 降低目标速度
            AddEffect { battle, user, target, tags(), 
                std::make_unique<Slow>(slow_duration, user, slow),
                slow_hit_rate
            }.invoke();
            // 消耗 1 个战技点
            CostSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

}