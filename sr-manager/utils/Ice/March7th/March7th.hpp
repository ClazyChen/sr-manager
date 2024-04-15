#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "March7thAeq.hpp"
#include "March7thPassive.hpp"
#include "March7thStars.hpp"

namespace sr {

    // 三月七
    class March7th : public Character {

    public:

        March7thA normal_attack;
        March7thE battle_skill;
        March7thE battle_skill_upgrade;
        March7thQ ultimate_skill;
        March7thQ ultimate_skill_upgrade;
        March7thQ ultimate_skill_upgrade2;

        const char* name() const { return "三月七"; }
        Path path() const { return Path::Preservation; }
        Type type() const { return Type::Ice; }
        int max_hp() const { return 160; }
        int max_mp() const { return 3; }
        int origin_mp() const { return 0; }
        int max_sp() const { return 0; }
        int origin_sp() const { return 0; }
        int attack() const { return 8; }
        int defense() const { return 10; }
        int speed() const { return  10; }
        int hit() const { return   0; }
        int resist() const { return   1; }
        int critical() const { return   0; }
        int penetration() const { return   0; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：终结技强化
                ultimate_skill_upgrade.mp_back = 1;
                ultimate_skill_upgrade2.mp_back = 1;
            }
            if (star >= 2) {
                // 2星：增加被动
                battle.triggers.add(std::make_unique<March7thS2>(unit));
            }
            if (star >= 3) {
                // 3星：终结技强化
                ultimate_skill_upgrade2.damage += 10;
                ultimate_skill_upgrade2.freeze += 10;
            }
            if (star >= 4) {
                // 4星：被动触发次数+1，反击威力提高30
            }
            if (star >= 5) {
                // 5星：战技强化
                battle_skill_upgrade.shield += 4;
            }
            if (star >= 6) {
                // 6星：增加被动
                battle.triggers.add(std::make_unique<March7thS6>(unit));
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            auto p1 = std::make_unique<March7thP1>(unit);
            if (unit.star >= 4) {
                p1->times_limit += 1;
                p1->damage += 30;
            }
            battle.triggers.add(std::make_unique<March7thP1Limit>(*p1));
            battle.triggers.add(std::move(p1));
            battle.triggers.add(std::make_unique<March7thP2>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                auto& target = battle_skill.target_ai(battle, unit);
                if (shield_value(target) == 0) {
                    EmitSkill{ battle, unit, target,
                        unit.star >= 5 ? battle_skill_upgrade : battle_skill }.invoke();
                    return;
                }
            }
            EmitSkill{ battle, unit, normal_attack.target_ai(battle, unit), normal_attack}.invoke();
        }

        int ultimate_skill_priority() const { return 8; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill{ battle, unit, ultimate_skill.target_ai(battle, unit),
                unit.star >= 3 ? ultimate_skill_upgrade2 :
                unit.star >= 1 ? ultimate_skill_upgrade : ultimate_skill }.invoke();
        }
    };

}