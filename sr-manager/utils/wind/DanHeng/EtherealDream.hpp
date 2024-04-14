#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 丹恒的终结技：洞天幻化·长梦一觉
    class EtherealDream : public Skill {
    public:
        int damage = 300;
        int slow_extra_damage = 80;

        const char* name() const { return "洞天幻化·长梦一觉"; }
        std::string description() const { return std::format(
            "消耗3点能量，对敌方单体造成威力{}的风伤害，削韧4。此技能对减速状态的目标威力提高{}。"
            , Painter::red(std::to_string(damage))
            , Painter::red(std::to_string(slow_extra_damage))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::Single };
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
            MpDown { battle, user, tags(), 3 }.invoke();
            // 对目标造成伤害
            if (target.effects.has(EffectType::Slow)) {
                Attack { battle, user, target, tags(), damage + slow_extra_damage, Type::Wind }.invoke();
            } else {
                Attack { battle, user, target, tags(), damage, Type::Wind }.invoke();
            }
            // 削减目标 6 点韧性
            ReduceToughness { battle, user, target, tags(), 4, Type::Wind }.invoke();
        }

    };

}