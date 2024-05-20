#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "TailblazerFireSkills.hpp"

namespace sr {

    // 开拓者・火
    class TailblazerFire : public Character {

    public:

        TailblazerFireA normal_attack;
        TailblazerFireE battle_skill;
        TailblazerFireQ ultimate_skill;

        const char* name() const { return "开拓者・火"; }
        Path path() const { return Path::Preservation; }
        Type type() const { return Type::Fire; }
        int max_hp() const { return 170; }
        int max_mp() const { return 3; }
        int origin_mp() const { return 0; }
        int max_sp() const { return 0; }
        int origin_sp() const { return 0; }
        int attack() const { return 9; }
        int defense() const { return 10; }
        int speed() const { return  9; }
        int hit() const { return   0; }
        int resist() const { return   0; }
        int critical() const { return   0; }
        int penetration() const { return   0; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 4) {
                unit.mp += 1;
            }
            if (star >= 6) {
                battle.triggers.add(std::make_unique <TailblazerFireS6>(unit));
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            battle.triggers.add(std::make_unique <TailblazerFireP1>(unit));
            battle.triggers.add(std::make_unique <TailblazerFireP2>(unit));
            battle.triggers.add(std::make_unique <TailblazerFireP3>(unit));
        }

        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill{ battle, unit, unit, battle_skill }.invoke();
            }
            else {
                EmitSkill{ battle, unit, normal_attack.target_ai(battle, unit), normal_attack }.invoke();
            }
        }

        int ultimate_skill_priority() const { return 9; }
        bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const {
            return unit.mp >= 3;
        }
        void ultimate(Battle& battle, BattleUnit& unit) const {
            EmitSkill{ battle, unit, ultimate_skill.target_ai(battle, unit), ultimate_skill }.invoke();
        }

    };

}