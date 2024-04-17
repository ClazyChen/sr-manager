#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "HertaAeq.hpp"
#include "HertaPassive.hpp"

namespace sr {

    // 黑塔
    class Herta : public Character {

    public:

        HertaA normal_attack;
        HertaA normal_attack_upgrade;
        HertaE battle_skill;
        HertaE battle_skill_upgrade;
        HertaQ ultimate_skill;
        HertaQ ultimate_skill_upgrade;
        HertaQ ultimate_skill_upgrade2;

        const char* name() const { return "黑塔"; }
        Path path() const { return Path::Erudition; }
        Type type() const { return Type::Ice; }
        int max_hp() const { return 140; }
        int max_mp() const { return 3; }
        int origin_mp() const { return 1; }
        int max_sp() const { return 0; }
        int origin_sp() const { return 0; }
        int attack() const { return 11; }
        int defense() const { return 6; }
        int speed() const { return  10; }
        int hit() const { return   0; }
        int resist() const { return   2; }
        int critical() const { return   1; }
        int penetration() const { return   2; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：普攻强化
                normal_attack_upgrade.damage_bonus = 40;
            }
            if (star >= 2) {
                // 2星：被动1强化
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade.damage += 10;
                battle_skill_upgrade.bonus_damage += 15;
            }
            if (star >= 4) {
                // 4星：被动1强化
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_skill_upgrade.damage += 15;
                ultimate_skill_upgrade.bonus_damage += 20;
                ultimate_skill_upgrade2.damage += 15;
                ultimate_skill_upgrade2.bonus_damage += 20;
            }
            if (star >= 6) {
                // 6星：终结技强化
                ultimate_skill_upgrade2.attack_up = 2;
            }
        }

        // 安装触发器
        int count = 0;
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            int& count_ref = const_cast<int&>(count);
            battle.triggers.add(std::make_unique<HertaP1>(unit, count_ref));
            battle.triggers.add(std::make_unique<HertaP2>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill{ battle, unit, battle_skill.target_ai(battle, unit),
                    unit.star >= 3 ? battle_skill_upgrade : battle_skill
                }.invoke();
            }
            else {
                EmitSkill{ battle, unit, normal_attack.target_ai(battle, unit), 
                    unit.star >= 1 ? normal_attack_upgrade : normal_attack 
                }.invoke();
            }
        }

        int ultimate_skill_priority() const { return 9; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill{ battle, unit, ultimate_skill.target_ai(battle, unit),
                unit.star >= 6 ? ultimate_skill_upgrade2 :
                unit.star >= 5 ? ultimate_skill_upgrade : ultimate_skill
            }.invoke();
        }
    };

}