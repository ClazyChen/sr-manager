#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "AstaAeq.hpp"
#include "AstaPassive.hpp"

namespace sr {

    // 艾丝妲
    class Asta : public Character {

    public:
        AstaA normal_attack;
        AstaE battle_skill;
        AstaE battle_skill_upgrade;
        AstaE battle_skill_upgrade2;
        AstaQ ultimate_skill;
        AstaQ ultimate_skill_upgrade;

        const char* name() const { return "艾丝妲"; }
        Path path() const { return Path::Harmony; }
        Type type() const { return Type::Fire; }
        int max_hp() const { return 160; }
        int max_mp() const { return   3; }
        int origin_mp() const { return   0; }
        int max_sp() const { return   5; }
        int origin_sp() const { return   0; }
        int attack() const { return  9; }
        int defense() const { return   7; }
        int speed() const { return  11; }
        int hit() const { return   0; }
        int resist() const { return   0; }
        int critical() const { return   1; }
        int penetration() const { return   2; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：战技强化
                battle_skill_upgrade.max_times += 1;
                battle_skill_upgrade2.max_times += 1;
            }
            if (star >= 2) {
                // 2星：终结技强化
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade2.damage += 10;
            }
            if (star >= 4) {
                // 4星：增加被动技能
                battle.triggers.add(std::make_unique<AstaS4>(unit));
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_skill_upgrade.speed_up += 1;
            }
            if (star >= 6) {
                // 6星：天赋强化
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            auto p1a = std::make_unique<AstaP1A>(unit);
            battle.triggers.add(std::make_unique<AstaP1B>(*p1a));
            battle.triggers.add(std::move(p1a));
            battle.triggers.add(std::make_unique<AstaP1C>(unit));
            battle.triggers.add(std::make_unique<AstaP2>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill{ battle, unit, battle_skill.target_ai(battle, unit),
                    unit.star >= 3 ? battle_skill_upgrade2 :
                    unit.star >= 1 ? battle_skill_upgrade : battle_skill
                }.invoke();
            }
            else {
                EmitSkill{ battle, unit, normal_attack.target_ai(battle, unit),
                     normal_attack
                }.invoke();
            }
        }

        int ultimate_skill_priority() const { return 5; }
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