#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/Break.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../effects/Fast.hpp"
#include "../../effects/Burn.hpp"
#include "../../effects/ResistDown.hpp"

namespace sr {

    // 姬子被动1 ：乘胜追击
    // 一名敌人被击破时追加攻击，对敌方全体造成伤害。
    struct HimekoP1 : public Trigger {
        int damage = 155;

        int speed_up = 0;

        HimekoP1(BattleUnit& owner, int speed_up = 0)
            : Trigger(owner, TriggerTime::Break), speed_up(speed_up) {}
        
        const char* name() const override {
            return "乘胜追击";
        }

        std::string description() const override {
            return std::format("一名敌人被击破时追加攻击，对敌方全体造成威力{}的火伤害，削韧1{}。", damage
            , (speed_up > 0 ? std::format("，速度提升{}点（2回合）。", speed_up) : ""));
        }

        std::string cli_text(Battle& battle) const override {
            return speed_up > 0 ? std::format("【{}】的速度提升{}点（2回合）。\n", target.colored_name(), speed_up) : "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Break& break_ = dynamic_cast<const Break&>(procedure);
            // 要求是敌人被击破即可
            return break_.target.side != target.side;
        }

        void on_trigger(Procedure& procedure) override {
            Break& break_ = dynamic_cast<Break&>(procedure);
            break_.on_trigger(*this, [&]{
                for (auto& unit : break_.battle.opponents(target)) {
                    if (unit->alive) {
                        Attack{
                            break_.battle, target, *unit,
                            { Tag::AdditionalSkill, Tag::All },
                            damage, Type::Fire
                        }.invoke();
                        ReduceToughness{
                            break_.battle, target, *unit,
                            { Tag::AdditionalSkill, Tag::All },
                            1, Type::Fire
                        }.invoke();
                    }
                }
                if (speed_up > 0) {
                    AddEffect{
                        break_.battle, break_.from, break_.from,
                        { Tag::AdditionalSkill, Tag::All },
                        std::make_unique<Fast>(2, break_.from, speed_up),
                        1
                    }.invoke();
                }
            });
        }

    };

    // 姬子被动2：星火
    // 释放攻击后，赋予目标灼烧效果，造成威力65的火伤害（3命中·2回合）。
    struct HimekoP2 : public Trigger {
        int damage = 65;
        int hit = 3;
        int duration = 2;

        HimekoP2(BattleUnit& owner)
            : Trigger(owner, TriggerTime::AttackEnd) {}

        const char* name() const override {
            return "星火";
        }

        std::string description() const override {
            return std::format("释放攻击后，赋予目标灼烧效果，造成威力{}的火伤害（{}命中·{}回合）。", damage, hit, duration);
        }

        std::string cli_text(Battle& battle) const override {
            return "";
        }

        bool is_triggrable(const Procedure& procedure) override {
            const Attack& attack = dynamic_cast<const Attack&>(procedure);
            return &attack.attacker == &target;
        }

        void on_trigger(Procedure& procedure) override {
            Attack& attack = dynamic_cast<Attack&>(procedure);
            AddEffect {
                attack.battle, target, attack.defender,
                { Tag::Burn },
                std::make_unique<Burn>(duration, target, damage),
                hit
            }.invoke();
        }

    };

    // 姬子被动3：不完全燃烧
    // 战斗开始时，敌方全体降低1点火抗性（6命中·2回合）。
    struct HimekoP3 : public Trigger {
        int reduce = 1;
        int hit = 6;
        int duration = 2;

        HimekoP3(BattleUnit& owner)
            : Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override {
            return "不完全燃烧";
        }

        std::string description() const override {
            return std::format("战斗开始时，敌方全体降低{}点火抗性（{}命中·{}回合）。", reduce, hit, duration);
        }

        std::string cli_text(Battle& battle) const override {
            std::string temp = "";
            for (const auto& unit : battle.opponents(target)) {
                temp += std::format("【{}】的火抗性降低{}点。\n", unit->colored_name(), reduce);
            }
            return temp;
        }

        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&]{
                for (auto& unit : game_start.battle.opponents(target)) {
                    AddEffect {
                        game_start.battle, target, *unit,
                        {  },
                        std::make_unique<ResistDown>(duration, target, Type::Fire, reduce),
                        hit
                    }.invoke();
                }
            });
        }

    };

}