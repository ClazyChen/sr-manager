#pragma once

#include "Procedure.hpp"

namespace sr {

    // 实现释放技能这一基本过程
    struct EmitSkill : public Procedure {
        BattleUnit& user;         // 使用者
        BattleUnit& target;       // 目标
        const Skill& skill;             // 技能

        EmitSkill(Battle& battle, BattleUnit& user, BattleUnit& target, const Skill& skill)
            : Procedure(battle, skill.tags()), user(user), target(target), skill(skill) {}

        void apply() override {
            if (!user.alive) return;
            if (!target.alive) return;
            battle.interface.print(std::format("【{}】对【{}】释放了技能【{}】。"
                , user.colored_name(), target.colored_name(), skill.name()
            ));
            battle.invoke(TriggerTime::SkillBegin, *this);
            skill.apply(battle, user, target);
            battle.invoke(TriggerTime::SkillEnd, *this);
        }

    };


}