#pragma once

#include "../../Trigger.hpp"
#include "../../../battle/GameStart.hpp"
#include "../../../battle/HpDown.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/TempAtk.hpp"

namespace sr {

    // 黑塔被动1：还是我来吧
    // 一名敌人生命降低到50%以下时追加攻击，对敌方全体造成威力80的冰伤害，削韧1。
    struct HertaP1 : public Trigger {
        int damage = 80;
        int& count;
        int crt_up = 1;
        int pen_up = 1;

        HertaP1(BattleUnit& owner, int& count):
            Trigger(owner, TriggerTime::HpDownEnd), count(count) {}

        const char* name() const override {
            return "还是我来吧";
        }

        std::string description() const override {
            return std::format("一名敌人生命降低到50%以下时追加攻击，对敌方全体造成威力{}的冰伤害，削韧1。", damage);
        }

        std::string cli_text(Battle& battle) const override {
            std::string temp = "";
            int count_n = count + 1;
            if (count_n % 2 == 0 && target.star >= 2) {
                temp += std::format("【{}】的暴击提升{}点。\n", target.colored_name(), crt_up);
            }
            if (count_n % 2 == 1 && target.star >= 4) {
                temp += std::format("【{}】的穿甲提升{}点。\n", target.colored_name(), pen_up);
            }
            return temp;
        }

        bool is_triggrable(const Procedure& procedure) override {
            const HpDown& hp_down = dynamic_cast<const HpDown&>(procedure);
            return hp_down.target.side != target.side &&
                hp_down.target.hp <= hp_down.target.max_hp / 2 &&
                hp_down.hp_before_down > hp_down.target.max_hp / 2;
        }

        void on_trigger(Procedure& procedure) override {
            HpDown& hp_down = dynamic_cast<HpDown&>(procedure);
            hp_down.on_trigger(*this, [&] {
                for (auto& unit : hp_down.battle.opponents(target)) {
                    if (unit->alive) {
                        Attack{
                            hp_down.battle, target, *unit,
                            { Tag::AdditionalSkill, Tag::All },
                            damage, Type::Ice
                        }.invoke();
                        ReduceToughness{
                            hp_down.battle, target, *unit,
                            { Tag::AdditionalSkill, Tag::All },
                            1, Type::Ice
                        }.invoke();
                    }
                }
                });
            count++;
            if (count % 2 == 0 && target.star >= 2) {
                target.critical += crt_up;
            }
            if (count % 2 == 1 && target.star >= 4) {
                target.penetration += pen_up;
            }
        }
    };

    // 黑塔被动2：可以优化一下
    // 战斗开始时，攻击提升4点（3回合）。
    struct HertaP2 : public Trigger {
        int attack_up = 4;
        int attack_up_duration = 3;

        // 构造函数
        HertaP2(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        // 名称
        const char* name() const override {
            return "可以优化一下";
        }

        // 描述
        std::string description() const override {
            return std::format("战斗开始时，攻击提升{}点（{}回合）。"
                , Painter::cyan(std::to_string(attack_up))
                , attack_up_duration);
        }

        // CLI 中显示的文本
        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】的攻击提升{}点（{}回合）。\n"
                , target.colored_name()
                , attack_up
                , attack_up_duration);
        }

        // 是否可以触发
        bool is_triggrable(const Procedure& procedure) override {
            return true;
        }

        // 触发效果
        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                AddEffect{
                    game_start.battle, target, target, {},
                    std::make_unique<TempAtk>(attack_up_duration, target, attack_up)
                }.invoke();
                });
        }
    };

}