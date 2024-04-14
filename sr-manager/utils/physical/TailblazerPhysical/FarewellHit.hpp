#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 开拓者（物理）的普攻技能：再见安打
    class FarewellHit : public Skill {
    public:
        int damage = 100;

        const char* name() const { return "再见安打"; }
        std::string description() const { return std::format(
            "对敌方单体造成威力{}的物理伤害，削韧2。"
            , Painter::red(std::to_string(damage))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack {
                    battle, user, u, tags(), damage, Type::Physical
                });
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack { battle, user, target, tags(), damage, Type::Physical }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness { battle, user, target, tags(), 2, Type::Physical }.invoke();
            // 获得 1 个战技点
            GetSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

}