#pragma once

#include "../../../battle/BattleUnit.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "CloudlancerArtNorthWind.hpp"
#include "CloudlancerArtTorrent.hpp"
#include "EtherealDream.hpp"
#include "SuperiorityOfReach.hpp"
#include "SplittingSpearhead.hpp"
#include "DanHengStars.hpp"

namespace sr {

    // 丹恒
    class DanHeng : public Character {

    public:
        CloudlancerArtNorthWind normal_attack;
        CloudlancerArtTorrent battle_skill;
        CloudlancerArtTorrent battle_skill_upgrade;
        CloudlancerArtTorrent battle_skill_upgrade_2;
        EtherealDream ultimate_skill;
        EtherealDream ultimate_skill_upgrade;

        const char* name() const { return "丹恒"; }
        Path path() const        { return Path::Hunt; }
        Type type() const        { return Type::Wind; }
        int max_hp() const       { return 120; }
        int max_mp() const       { return   3; }
        int origin_mp() const    { return   0; }
        int max_sp() const       { return   0; }
        int origin_sp() const    { return   0; }
        int attack() const       { return  11; }
        int defense() const      { return   6; }
        int speed() const        { return  13; }
        int hit() const          { return   0; }
        int resist() const       { return   0; }
        int critical() const     { return   0; }
        int penetration() const  { return   2; }

        // 星级提升时的效果
        void add_star(Battle& battle, BattleUnit& unit, int star) {
            if (star >= 1) {
                // 1星：增加被动技能【穷高极天，亢盈难久】
                battle.triggers.add(std::make_unique<DanHengS1>(unit));
            }
            if (star >= 2) {
                // 2星：【寸长寸强】的冷却时间减少为 1
            }
            if (star >= 3) {
                // 3星：战技强化
                battle_skill_upgrade.damage += 25;
                battle_skill_upgrade_2.damage += 25;
            }
            if (star >= 4) {
                // 4星：增加被动技能【奋迅三昧，如日空居】
                battle.triggers.add(std::make_unique<DanHengS4>(unit));
            }
            if (star >= 5) {
                // 5星：终结技强化
                ultimate_skill_upgrade.damage += 30;
                ultimate_skill_upgrade.slow_extra_damage += 30;
            }
            if (star >= 6) {
                // 6星：战技触发的减速增加1点
                battle_skill_upgrade_2.slow += 1;
            }
        }

        // 安装触发器
        void add_triggers(Battle& battle, BattleUnit& unit) const {
            auto reach = std::make_unique<SuperiorityOfReach>(unit);
            battle.triggers.add(std::make_unique<SuperiorityOfReachCooldown>(*reach));
            battle.triggers.add(std::move(reach));
            battle.triggers.add(std::make_unique<SplittingSpearhead>(unit));
        }

        // 进行回合时的 AI
        void take_turn_ai(Battle& battle, BattleUnit& unit) const {
            if (unit.effects.has(EffectType::Freeze)) return;
            if (battle.skill_points(unit.side) > 0) {
                EmitSkill { battle, unit, battle_skill.target_ai(battle, unit), 
                unit.star >= 6 ? battle_skill_upgrade_2 : 
                unit.star >= 3 ? battle_skill_upgrade : battle_skill
                 }.invoke();
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