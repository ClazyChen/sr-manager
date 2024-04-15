#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "FarewellHit.hpp"
#include "RipHomeRun.hpp"
#include "StardustAce.hpp"
#include "PerfectPickoff.hpp"
#include "ImmortalThirdStrike.hpp"
#include "AFallingStar.hpp"
#include "AnUnwillingHost.hpp"
#include "ADestructingGlance.hpp"
#include "ATrailblazingWill.hpp"

namespace sr {

    // 开拓者（物理）
    class TailblazerPhysical : public Character {

    public:
        FarewellHit normal_attack;
        RipHomeRun battle_skill;
        RipHomeRun battle_skill_upgrade;
        StardustAce ultimate_skill;
        StardustAce ultimate_skill_upgrade;

        const char* name() const { return "开拓者·物理"; }
        Path path() const        { return Path::Destruction; }
        Type type() const        { return Type::Physical; }
        int max_hp() const       { return 175; }
        int max_mp() const       { return   3; }
        int origin_mp() const    { return   1; }
        int max_sp() const       { return   0; }
        int origin_sp() const    { return   0; }
        int attack() const       { return  11; }
        int defense() const      { return   6; }
        int speed() const        { return  10; }
        int hit() const          { return   0; }
        int resist() const       { return   0; }
        int critical() const     { return   0; }
        int penetration() const  { return   0; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：增加被动技能【坠临万界的星芒】
                battle.triggers.add(std::make_unique<AFallingStar>(unit));
            }
            if (star >= 2) {
                // 2星：增加被动技能【因缘假合的人身】
                battle.triggers.add(std::make_unique<AnUnwillingHost>(unit));
            }
            if (star >= 3) {
                // 3星：战技强化，对主目标威力提高25，对次要目标威力提高20
                battle_skill_upgrade.damage_main += 25;
                battle_skill_upgrade.damage_side += 20;
            }
            if (star >= 4) {
                // 4星：增加被动技能【凝眸毁灭的瞬间】
                battle.triggers.add(std::make_unique<ADestructingGlance>(unit));
            }
            if (star >= 5) {
                // 5星：终结技强化
                // 全胜·再见安打威力提高30
                // 全胜·安息全垒打对主目标威力提高25，对次要目标威力提高20
                ultimate_skill_upgrade.damage_single += 30;
                ultimate_skill_upgrade.damage_spread_main += 25;
                ultimate_skill_upgrade.damage_spread_side += 20;
            }
            if (star >= 6) {
                // 6星：增加被动技能【拓宇行天的意志】
                battle.triggers.add(std::make_unique<ATrailblazingWill>(unit));
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            battle.triggers.add(std::make_unique<ImmortalThirdStrike>(unit));
            battle.triggers.add(std::make_unique<PerfectPickoff>(unit));
        }

        // 进行回合时的 AI
        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill { battle, unit, battle_skill.target_ai(battle, unit), 
                unit.star >= 3 ? battle_skill_upgrade : battle_skill }.invoke();
            } else {
                EmitSkill { battle, unit, normal_attack.target_ai(battle, unit), normal_attack }.invoke();
            }
        }

        // 终结技能
        int ultimate_skill_priority() const { return 9; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill { battle, unit, ultimate_skill.target_ai(battle, unit), 
            unit.star >= 5 ? ultimate_skill_upgrade : ultimate_skill
             }.invoke();
        }
    };

}