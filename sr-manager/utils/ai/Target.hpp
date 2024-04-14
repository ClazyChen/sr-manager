#pragma once

#include "../Utils.hpp"
#include "../../battle/Battle.hpp"
#include "../../battle/Attack.hpp"

namespace sr::ai {

    // 智能选择目标的一系列参考方法
    
    // 造成伤害的期望值
    double attack_expectation(const Attack& inst) {
        // 这里所有实际情况都需要考虑
        auto attack = inst.attacker.attack * inst.damage / 10.0;
        auto penetrate = std::min(1.0, inst.attacker.penetration / 6.0);
        auto base_damage = attack - inst.defender.defense * 10 * (1.0 - penetrate);
        auto critical = std::min(1.0, inst.attacker.critical / 6.0);
        auto hp_damage = base_damage * (1.0 + critical / 2.0);
        // auto actual_hp_damage = std::max(hp_damage, static_cast<double>(inst.defender.hp));
        return hp_damage;
    }

    // 

}