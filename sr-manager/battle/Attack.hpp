#pragma once

#include "Procedure.hpp"
#include "HpDown.hpp"
#include "../utils/effects/AttackUpgrade.hpp"
#include "../utils/effects/ResistDown.hpp"
#include "../utils/effects/Shield.hpp"
#include <map>

namespace sr {

    // 发动一次攻击的类
    struct Attack : public Procedure {
        // 开始攻击时传入的参数
        BattleUnit& attacker;    // 攻击方
        BattleUnit& defender;    // 防御方
        int damage;              // 技能威力
        Type type;               // 技能属性

        int critical_rate;       // 暴击率
        int penetration_rate;    // 穿甲率

        // 攻击过程中计算的参数
        int hp_damage;           // 造成的伤害
        bool critical;           // 是否暴击
        bool penetrate;          // 是否穿甲
        int shielded;            // 护盾抵消的伤害

        // 构造函数
        Attack(Battle& battle, BattleUnit& attacker, BattleUnit& defender, std::vector<Tag> tags, int damage, Type type)
            : Procedure(battle, tags), attacker(attacker), defender(defender), damage(damage), type(type) {
                critical_rate = attacker.critical;
                penetration_rate = attacker.penetration;
                for (auto& effect : attacker.effects) {
                    if (effect->type() == EffectType::AttackUpgrade) {
                        auto& attack_upgrade = dynamic_cast<AttackUpgrade&>(*effect);
                        if (has_tag(tags, attack_upgrade.attack_tag)) {
                            damage += attack_upgrade.damage;
                        }
                    }
                }
            }

    private:    
        // 计算攻击的效果
        void cal_effect() {
            // 实际攻击 = 攻击 * 威力 / 10
            auto attack = attacker.attack * damage / 10;
            // 判断是否穿甲
            penetrate = Random::dice(penetration_rate);
            // 基础伤害 = 实际攻击 - 防御 * 10 *（穿甲 ? 0 : 1）
            auto base_damage = attack - std::max(0, defender.defense) * 10 * (penetrate ? 0 : 1);
            base_damage = std::max(base_damage, 1);
            // 元素伤害抗性影响
            for (auto& effect : defender.effects) {
                if (effect->type() == EffectType::ResistDown) {
                    auto& resist_down = dynamic_cast<ResistDown&>(*effect);
                    if (resist_down.element_type == type) {
                        base_damage = base_damage + resist_down.damage;
                    }
                }
            }
            // 判断是否暴击
            critical = Random::dice(critical_rate);
            // 如果暴击，伤害 = 基础伤害 * 3 / 2    否则，伤害 = 基础伤害
            if (critical) {
                hp_damage = base_damage * 3 / 2;
            } else {
                hp_damage = base_damage;
            }
        }

    public:

        // tag -> 显示
        static inline std::map <Tag, std::string> tag_name = {
            { Tag::Bleed, "裂伤" },
            { Tag::WindShear, "风化" },
            { Tag::Burn, "灼烧" },
            { Tag::Freeze, "冻结" },
            { Tag::Shock, "触电" }
        };

        // 执行攻击
        void apply() override {
            if (!defender.alive) return;
            battle.invoke(TriggerTime::AttackBegin, *this);
            // 计算攻击效果
            cal_effect();
            // 特殊效果的伤害显示
            std::string from = "";
            for (auto& [tag, name] : tag_name) {
                if (has_tag(tags, tag)) {
                    from += std::format("（{}）", name);
                }
            }
            battle.interface.print(std::format("【{}】对【{}】造成了 {:0.1f} 点 {} 伤害。{}{}",
                attacker.colored_name(),
                defender.colored_name(),
                hp_damage / 10.0,
                type_name(type),
                Painter::green(
                    critical && penetrate ? "暴击且穿甲！" :
                    critical ? "暴击！" :
                    penetrate ? "穿甲！" :
                    ""
                ),
                from
            ));
            // 计算护盾
            shielded = 0;
            for (int i = 0; i < defender.effects.size(); i++) {
                if (defender.effects[i]->type() == EffectType::Shield) {
                    auto& shield = dynamic_cast<Shield&>(*defender.effects[i]);
                    if (shield.shield <= hp_damage) {
                        // 护盾消失
                        shielded = std::max(shielded, shield.shield);
                        // 删除护盾效果
                        defender.effects.erase(defender.effects.begin() + i);
                        i--;
                    } else {
                        // 护盾削减
                        shielded = std::max(shielded, hp_damage);
                        shield.shield -= hp_damage;
                    }
                }
            }
            if (shielded > 0) {
                battle.interface.print(std::format("【{}】的护盾抵消了 {:0.1f} 点伤害。",
                    defender.colored_name(),
                    shielded / 10.0
                ));
            }
            // 伤害减少
            hp_damage -= shielded;
            if (hp_damage > 0) {
                // 触发生命值下降效果
                HpDown{
                    battle, defender, tags, hp_damage
                }.invoke();
                // 如果造成目标死亡，触发击杀时机
                if (!defender.alive) {
                    battle.invoke(TriggerTime::Kill, *this);
                }
            }
            battle.invoke(TriggerTime::AttackEnd, *this);
        }

    };

}