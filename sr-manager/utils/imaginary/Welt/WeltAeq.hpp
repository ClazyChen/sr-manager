#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/Slow.hpp"
#include "../../effects/DefDown.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 瓦尔特A：重力压制
    // 对敌方单体造成威力100的虚数伤害，削韧2。
    struct WeltA : public Skill {
        int damage = 100;

        const char* name() const { return "重力压制"; }
        std::string description() const {
            return std::format(
            "对敌方单体造成威力{}的虚数伤害，削韧2。"
            , Painter::red(std::to_string(damage))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Imaginary
                });
            });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack { battle, user, target, tags(), damage, Type::Imaginary }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness { battle, user, target, tags(), 2, Type::Imaginary }.invoke();
            // 获得 1 个战技点
            GetSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

    // 瓦尔特E：虚空断界
    // 对敌方随机单体造成威力70的虚数伤害（最多3次），削韧2。目标速度降低1点（2命中・1回合）。
    struct WeltE : public Skill {
        int damage = 70;
        int times = 3;
        int speed_down = 1;
        int hit = 2;
        int duration = 1;

        const char* name() const { return "虚空断界"; }
        std::string description() const {
            return std::format(
            "对敌方随机单体造成威力{}的虚数伤害（最多{}次），削韧2。目标速度降低{}点（{}命中・{}回合）。"
            , Painter::red(std::to_string(damage))
            , Painter::red(std::to_string(times))
            , Painter::red(std::to_string(speed_down))
            , Painter::red(std::to_string(hit))
            , Painter::red(std::to_string(duration))
        ); }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Random};
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            for (int i = 0; i < times; i++) {
                // 如果游戏已经结束，停止攻击
                if (battle.finished()) break;
                // 随机选择目标
                auto& target = *battle.random_target(battle.opponents(user));
                // 对目标造成伤害
                Attack { battle, user, target, tags(), damage, Type::Imaginary }.invoke();
                // 削减目标 2 点韧性
                ReduceToughness { battle, user, target, tags(), 2, Type::Imaginary }.invoke();
                // 降低目标速度
                AddEffect { battle, user, target, tags(), std::make_unique<Slow>(duration, user, speed_down), hit }.invoke();
            }
            // 消耗 1 个战技点
            CostSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }

    };

    // 瓦尔特Q：拟似黑洞
    // 消耗3点能量，对敌方全体造成威力150的虚数伤害，削韧4。目标速度降低1点并推条4点（6命中・1回合），防御降低1点（6命中・2回合）。
    struct WeltQ : public Skill {
        int damage = 150;
        int speed_down = 1;
        int push = 4;
        int def_down = 1;
        int hit = 6;
        int duration_speed = 1;
        int duration_def = 2;

        const char* name() const { return "拟似黑洞"; }
        std::string description() const {
            return std::format(
            "消耗3点能量，对敌方全体造成威力{}的虚数伤害，削韧4。目标速度降低{}点并推条{}点（{}命中・{}回合），防御降低{}点（{}命中・{}回合）。"
            , Painter::red(std::to_string(damage))
            , speed_down
            , push
            , hit
            , duration_speed
            , def_down
            , hit
            , duration_def
        ); }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            // 全体攻击，随便选择一个目标即可
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            MpDown { battle, user, tags(), 3 }.invoke();
            // 对目标造成伤害
            for (auto& u : battle.opponents(user)) {
                if (!u->alive) continue;
                Attack { battle, user, *u, tags(), damage, Type::Imaginary }.invoke();
                // 削减目标 4 点韧性
                ReduceToughness { battle, user, *u, tags(), 4, Type::Imaginary }.invoke();
                // 降低目标速度
                auto add_effect = AddEffect{ battle, user, *u, tags(), std::make_unique<Slow>(duration_speed, user, speed_down), hit };
                add_effect.invoke();
                if (add_effect.is_hit) {
                    // 如果命中则推条
                    battle.speed_bar.push(*u, push);
                }
                // 降低目标防御
                AddEffect { battle, user, *u, tags(), std::make_unique<DefDown>(duration_def, user, def_down), hit }.invoke();
            }
        }
    };

}