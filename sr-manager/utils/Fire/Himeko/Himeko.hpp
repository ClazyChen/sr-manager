#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "HimekoAeq.hpp"
#include "HimekoPassive.hpp"
#include "HimekoStars.hpp"

namespace sr {

    // 姬子
    class Himeko : public Character {

    public:
        HimekoA normal_attack;  // 姬子的普通攻击
        HimekoE battle_skill;   // 姬子的战斗技能
        HimekoE battle_skill_upgrade;
        HimekoQ ultimate_skill;       // 姬子的大招
        HimekoQ ultimate_upgrade;
        HimekoQ ultimate_upgrade_2;


        const char* name() const { return "姬子"; }
        Path path() const        { return Path::Erudition; }
        Type type() const        { return Type::Fire; }
        int max_hp() const       { return 160; }
        int max_mp() const       { return   3; }
        int origin_mp() const    { return   0; }
        int max_sp() const       { return   0; }
        int origin_sp() const    { return   0; }
        int attack() const       { return  14; }
        int defense() const      { return   4; }
        int speed() const        { return   9; }
        int hit() const          { return   0; }
        int resist() const       { return   1; }
        int critical() const     { return   0; }
        int penetration() const  { return   2; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：天赋强化
            }
            if (star >= 2) {
                battle.triggers.add(std::make_unique<HimekoS2>(unit));
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade.damage_main += 20;
                battle_skill_upgrade.damage_side += 10;
                battle_skill_upgrade.damage_main_burn += 25;
                battle_skill_upgrade.damage_side_burn += 15;
            }
            if (star >= 4) {
                battle.triggers.add(std::make_unique<HimekoS4>(unit));
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_upgrade.damage += 20;
            }
            if (star >= 6) {
                // 6星：终结技强化
                ultimate_upgrade_2.extra_damage = 100;
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            battle.triggers.add(std::make_unique<HimekoP1>(unit, (
                unit.star >= 1 ? 2 : 0
            )));
            battle.triggers.add(std::make_unique<HimekoP2>(unit));
            battle.triggers.add(std::make_unique<HimekoP3>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill { battle, unit, battle_skill.target_ai(battle, unit), 
                    unit.star >= 3 ? battle_skill_upgrade : battle_skill
                }.invoke();
            } else {
                EmitSkill { battle, unit, battle_skill.target_ai(battle, unit), normal_attack }.invoke();
            }
        }

        int ultimate_skill_priority() const { return 9; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill { battle, unit, ultimate_skill.target_ai(battle, unit), 
                unit.star >= 6 ? ultimate_upgrade_2 : 
                unit.star >= 5 ? ultimate_upgrade : ultimate_skill
            }.invoke();
        }
    };

}