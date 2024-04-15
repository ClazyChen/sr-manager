#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/TurnBegin.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/Freeze.hpp"

namespace sr {

    // 三月七被动1：少女的特权
    // 持有护盾的我方目标受到攻击时反击，追加威力100的冰伤害，削韧2。此技能每回合至多触发2次。
    struct March7thP1 : public Trigger {
        int damage = 100;
        int times = 0;
        int times_limit = 2;

        March7thP1(BattleUnit& owner)
            : Trigger(owner, TriggerTime::AttackEnd) {}

        const char* name() const override {
            return "少女的特权";
        }

        std::string description() const override {
            return std::format("持有护盾的我方目标受到攻击时反击，追加威力{}的冰伤害，削韧2。此技能每回合至多触发{}次。", damage, times_limit);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            // 要求目标有护盾且是我方目标且未触发次数未达上限
            return attack.defender.side == target.side && attack.shielded > 0 && times < times_limit;
        }

        void on_trigger(Procedure& procedure) override {
            Attack& attack = dynamic_cast<Attack&>(procedure);
            times++;
            attack.on_trigger(*this, [&] {
                Attack{
                    attack.battle, target, attack.attacker,
                    { Tag::AdditionalSkill, Tag::Single },
                    damage, Type::Ice
                }.invoke();
                ReduceToughness{ attack.battle, target, attack.attacker, 
                    { Tag::AdditionalSkill, Tag::Single }, 2, Type::Ice }.invoke();
            });
        }
    };

    // 用来控制三月七被动1的触发次数的辅助类
    struct March7thP1Limit : public Trigger {
        March7thP1& p1;

        // 构造函数
        March7thP1Limit(March7thP1& p1)
            : Trigger(p1.target, TriggerTime::TurnBegin), p1(p1) {}

        // 自己的回合开始时触发
        bool is_triggrable(const Procedure& procedure) override {
            // 强制转换为 TurnBegin& 类型
            const TurnBegin& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            // 要求是自己的回合
            return &turn_begin.target == &target;
        }

        void on_trigger(Procedure& procedure) override {
            p1.times = 0;
        }

    };

    // 三月七被动2：冻人的瞬间
    // 战斗开始时，随机敌人附加威力65的冻结状态（6命中・1回合）。
    struct March7thP2 : public Trigger {
        int damage = 65;

        March7thP2(BattleUnit& owner)
            : Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "冻人的瞬间";
        }

        std::string description() const override {
            return std::format("战斗开始时，随机敌人附加威力{}的冻结状态（6命中・1回合）。", damage);
        }

        std::string cli_text(Battle& battle) const override {
            return ""; // 不需要显示
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                Battle& battle = game_start.battle;
                // 随机选择一个敌人
                BattleUnit& u = *battle.random_target(battle.opponents(target));
                // 附加冻结状态
                AddEffect{ battle, target, u, { Tag::Single },
                    std::make_unique<Freeze>(1, target, damage), 6 }.invoke();
                });
        }
    };
}