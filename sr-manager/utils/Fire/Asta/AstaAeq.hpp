#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../effects/Burn.hpp"
#include "../../effects/Fast.hpp"

namespace sr {

    // 艾丝妲A：光谱射线
    // 对敌方单体造成威力100的火伤害，附加威力75的灼烧状态（5命中·3回合），削韧2。
    struct AstaA : public Skill {
        int damage = 100;
        int burn_damage = 75;
        int burn_hit = 5;
        int burn_duration = 3;

        const char* name() const { return "光谱射线"; }
        std::string description() const {
            return std::format(
                "对敌方单体造成威力{}的火伤害，削韧2。附加威力{}的灼烧状态（{}命中·{}回合）。"
                , Painter::red(std::to_string(damage))
                , Painter::red(std::to_string(burn_damage))
                , burn_hit, burn_duration
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Fire
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack{ battle, user, target, tags(), damage, Type::Fire }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness{ battle, user, target, tags(), 2, Type::Fire }.invoke();
            // 附加灼烧状态
            AddEffect{ battle, user, target, tags(), std::make_unique<Burn>(burn_duration, user, burn_damage), burn_hit }.invoke();
            // 获得 1 个战技点
            GetSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
            // 被动：回复 1 点技力
            user.sp = std::min(user.sp + 1, user.max_sp);
        }
    };

    // 艾丝妲E：流星群落
    // 对敌方随机单体造成威力75的火伤害（最多5次），削韧1。
    struct AstaE : public Skill {
        int damage = 75;
        int max_times = 5;
        int toughness_reduce = 3;

        const char* name() const { return "流星群落"; }
        std::string description() const {
            return std::format(
                "对敌方随机单体造成威力{}的火伤害（最多{}次），削韧1。"
                , Painter::red(std::to_string(damage))
                , max_times
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::Random };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            std::vector<int> targets;
            for (int i = 0; i < max_times; i++) {
                // 如果游戏已经结束，停止攻击
                if (battle.finished()) break;
                // 随机选择目标
                auto& target = *battle.random_target(battle.opponents(user));
                if (std::find(targets.begin(), targets.end(), target.position) == targets.end()) {
                    targets.push_back(target.position);
                }
                // 对目标造成伤害
                Attack{ battle, user, target, tags(), damage, Type::Fire }.invoke();
                // 削减目标韧性
                ReduceToughness{ battle, user, target, tags(), 1, Type::Fire }.invoke();
            }
            // 消耗 1 个战技点
            CostSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
            // 被动：回复 targets.size() 点技力
            user.sp = std::min(user.sp + (int)targets.size(), user.max_sp);
        }
    };

    // 艾丝妲Q：星空祝言
    // 消耗3点能量，我方全体速度提升4点（2回合）。
    struct AstaQ : public Skill {
        int cost = 3;
        int speed_up = 4;
        int duration = 2;

        const char* name() const { return "星空祝言"; }
        std::string description() const {
            return std::format(
                "消耗{}点能量，我方全体速度提升{}点（{}回合）。"
                , cost, speed_up, duration
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return user;
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 消耗 3 点能量
            MpDown{ battle, user, tags(), cost }.invoke();
            // 提升我方全体速度
            for (auto& u : battle.teammates(user)) {
                if (u->alive) {
                    AddEffect{ battle, user, *u, tags(), std::make_unique<Fast>(duration, user, speed_up) }.invoke();
                }
            }
            // 2星被动：终结技回复 3 点技力
            if (user.star >= 2) {
                user.sp = std::min(user.sp + 3, user.max_sp);
            }
        }
    };
}