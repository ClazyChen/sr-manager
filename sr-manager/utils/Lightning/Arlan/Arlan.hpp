#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "ArlanAeq.hpp"
#include "ArlanPassive.hpp"

namespace sr {

    // 阿兰
    class Arlan : public Character {

    public:
        ArlanA normal_attack;
        ArlanE battle_skill;
        ArlanE battle_skill_upgrade;
        ArlanE battle_skill_upgrade2;
        ArlanQ ultimate_skill;
        ArlanQ ultimate_skill_upgrade;

        const char* name() const { return "阿兰"; }
        Path path() const { return Path::Destruction; }
        Type type() const { return Type::Lightning; }
        int max_hp() const { return 175; }
        int max_mp() const { return 3; }
        int origin_mp() const { return 1; }
        int max_sp() const { return 0; }
        int origin_sp() const { return 0; }
        int attack() const { return 11; }
        int defense() const { return 3; }
        int speed() const { return  11; }
        int hit() const { return   0; }
        int resist() const { return   2; }
        int critical() const { return   0; }
        int penetration() const { return   0; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：战技强化
                battle_skill_upgrade.bonus_damage = 25;
                battle_skill_upgrade2.bonus_damage = 25;
            }
            if (star >= 2) {
                // 2星：增加被动
                battle.triggers.add(std::make_unique<ArlanS2>(unit));
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade.damage += 25;
            }
            if (star >= 4) {
                // 4星：增加被动
                battle.triggers.add(std::make_unique<ArlanS4>(unit));
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_skill_upgrade.damage_main += 25;
                ultimate_skill_upgrade.damage_side += 10;
            }
            if (star >= 6) {
                // 6星：终结技强化
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            battle.triggers.add(std::make_unique<ArlanP1>(unit));
            battle.triggers.add(std::make_unique<ArlanP2>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            EmitSkill{ battle, unit, battle_skill.target_ai(battle, unit),
                    unit.star >= 3 ? battle_skill_upgrade2 :
                    unit.star >= 1 ? battle_skill_upgrade :
                    battle_skill
            }.invoke();
        }

        int ultimate_skill_priority() const { return 9; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill{ battle, unit, ultimate_skill.target_ai(battle, unit),
                unit.star >= 5 ? ultimate_skill_upgrade : ultimate_skill
            }.invoke();
        }

    };
}