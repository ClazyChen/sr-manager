#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "WeltAeq.hpp"
#include "WeltPassive.hpp"
#include "WeltStars.hpp"

namespace sr {

    // 瓦尔特
    class Welt : public Character {

    public:

        WeltA normal_attack;
        WeltE battle_skill;
        WeltE battle_skill_upgrade;
        WeltE battle_skill_upgrade_2;
        WeltE battle_skill_upgrade_3;
        WeltQ ultimate_skill;
        WeltQ ultimate_skill_upgrade;

        const char* name() const { return "瓦尔特"; }
        Path path() const { return Path::Nihility; }
        Type type() const { return Type::Imaginary; }
        int max_hp() const { return 160; }
        int max_mp() const { return   3; }
        int origin_mp() const { return   1; }
        int max_sp() const { return   0; }
        int origin_sp() const { return   0; }
        int attack() const { return  12; }
        int defense() const { return   5; }
        int speed() const { return  11; }
        int hit() const { return   0; }
        int resist() const { return   1; }
        int critical() const { return   0; }
        int penetration() const { return   1; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：增加被动技能
                battle.triggers.add(std::make_unique<WeltS1>(unit));
            }
            if (star >= 2) {
                // 2星：能量+1
                unit.mp += 1;
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade.damage += 10;
                battle_skill_upgrade_2.damage += 10;
                battle_skill_upgrade_3.damage += 10;
            }
            if (star >= 4) {
                // 4星：战技强化
                battle_skill_upgrade_2.hit += 1;
                battle_skill_upgrade_3.hit += 1;
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_skill_upgrade.damage += 10;
            }
            if (star >= 6) {
                // 6星：战技强化
                battle_skill_upgrade_3.times += 1;
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            // battle.triggers.add(std::make_unique<WeltP1>(unit));
            battle.triggers.add(std::make_unique<WeltP2>(unit));
            battle.triggers.add(std::make_unique<WeltP3>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill{ battle, unit, battle_skill.target_ai(battle, unit), 
                                   unit.star >= 6 ? battle_skill_upgrade_3 : 
                                   unit.star >= 4 ? battle_skill_upgrade_2 : 
                                   unit.star >= 3 ? battle_skill_upgrade : battle_skill }.invoke();
            }
            else {
                EmitSkill{ battle, unit, battle_skill.target_ai(battle, unit), normal_attack }.invoke();
            }
        }

        int ultimate_skill_priority() const { return 8; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill{ battle, unit, ultimate_skill.target_ai(battle, unit), 
                                          unit.star >= 5 ? ultimate_skill_upgrade : ultimate_skill }.invoke();
        }

    };

}