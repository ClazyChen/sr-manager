#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 丹恒的普攻技能：云骑枪术·朔风
    class CloudlancerArtNorthWind : public Skill {
    public:
        int damage = 100;
        int slow_extra_damage = 40;

        const char* name() const { return "云骑枪术·朔风"; }
        std::string description() const { return std::format(
            "对敌方单体造成威力{}的风伤害，削韧2。此技能对减速状态的目标威力提高{}。"
            , Painter::red(std::to_string(damage))
            , Painter::red(std::to_string(slow_extra_damage))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                if (u.effects.has(EffectType::Slow)) {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage + slow_extra_damage, Type::Wind
                    });
                } else {
                    return ai::attack_expectation(Attack {
                        battle, user, u, tags(), damage, Type::Wind
                    });
                }
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            if (target.effects.has(EffectType::Slow)) {
                Attack { battle, user, target, tags(), damage + slow_extra_damage, Type::Wind }.invoke();
            } else {
                Attack { battle, user, target, tags(), damage, Type::Wind }.invoke();
            }
            // 削减目标 2 点韧性
            ReduceToughness { battle, user, target, tags(), 2, Type::Wind }.invoke();
            // 获得 1 个战技点
            GetSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

}