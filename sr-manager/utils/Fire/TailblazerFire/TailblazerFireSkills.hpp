#pragma once

#include "../../Utils.hpp"
#include "../../Trigger.hpp"
#include "../../../battle/Attack.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

#include "../../../battle/AddEffect.hpp"
#include "../../../battle/EmitSkill.hpp"
#include "../../effects/TempDef.hpp"
#include "../../effects/TempAtk.hpp"
#include "../../effects/Taunt.hpp"
#include "../../../battle/TurnBegin.hpp"
#include "../../effects/Shield.hpp"
#include "../../../battle/TurnEnd.hpp"
#include "../../../battle/GameStart.hpp"

namespace sr {

    // 开拓者・火A：穿彻坚冰的烈芒
    // 对敌方单体造成威力100的火伤害。
    struct TailblazerFireA : public Skill {

        int damage = 100;
        int damage_s1 = 50; // 1 星加成
        
        const char* name() const { return "穿彻坚冰的烈芒"; }
        std::string description() const {
            return std::format(
                "对敌方单体造成威力{}的火伤害，削韧2。"
                , Painter::red(std::to_string(damage))
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        int _damage(BattleUnit& user) const {
            return damage + (user.star >= 1 ? damage_s1 : 0);
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), _damage(user), Type::Fire
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack{ battle, user, target, tags(), _damage(user), Type::Fire }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness{ battle, user, target, tags(), 2, Type::Fire }.invoke();
            // 获得 1 个战技点
            GetSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 开拓者・火E：炽燃不灭的琥珀
    // 防御提高7点（1回合），并使我方全体防御提高2点（1回合），嘲讽敌方全体（6命中・1回合）。
    struct TailblazerFireE : public Skill {
        
        int defense_up = 7;
        int defense_up_s3 = 2; // 3 星加成
        int defense_up_all = 2;

        int taunt_hit = 6;
        int duration = 1;

        const char* name() const { return "炽燃不灭的琥珀"; }
        
        std::vector<Tag> tags() const {
            return { Tag::BattleSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return user;
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 为自己添加 7 点临时防御
            AddEffect{ battle, user, user, tags(), 
                std::make_unique<TempDef>(duration, user, defense_up
                + (user.star >= 3 ? defense_up_s3 : 0)) }.invoke();
            // 为全体友方添加 2 点临时防御
            for (auto& u : battle.teammates(user)) {
                if (u->alive) {
                    AddEffect{ battle, user, *u, tags(), 
                        std::make_unique<TempDef>(duration, user, defense_up_all) }.invoke();
                }
            }
            // 为敌方全体添加嘲讽效果
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    AddEffect{ battle, user, *u, tags(), std::make_unique<Taunt>(duration, user), taunt_hit }.invoke();
                }
            }
            // 消耗 1 个战技点
            CostSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }

    };

    // 开拓者・火Q：陷阵无回的炎枪
    // 消耗3点能量，对敌方全体造成威力250的火伤害，削韧4。
    struct TailblazerFireQ : public Skill {
        
        int damage = 250;
        int damage_s5 = 25; // 5 星加成

        const char* name() const { return "陷阵无回的炎枪"; }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        int _damage(BattleUnit& user) const {
            return damage + (user.star >= 5 ? damage_s5 : 0);
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            MpDown{ battle, user, tags(), 3 }.invoke();
            // 对敌方全体造成伤害
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    Attack{ battle, user, *u, tags(), _damage(user), Type::Fire }.invoke();
                    // 削减目标 4 点韧性
                    ReduceToughness{ battle, user, *u, tags(), 4, Type::Fire }.invoke();
                }
            }
        }
    };

    // 开拓者·火被动技能1：行胜思
    // 回合开始时，如果持有护盾，攻击提高2点（1回合）。
    struct TailblazerFireP1 : public Trigger {
        int atk_up = 2;

        TailblazerFireP1(BattleUnit& owner) : 
            Trigger(owner, TriggerTime::TurnBegin) {}
            
        const char* name() const { return "行胜思"; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_begin = dynamic_cast<const TurnBegin&>(procedure);
            return &turn_begin.target == &target && shield_value(target) > 0;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_begin = dynamic_cast<TurnBegin&>(procedure);
            turn_begin.on_trigger(*this, [&] {
                AddEffect{ turn_begin.battle, target, target, {}, 
                    std::make_unique<TempAtk>(1, target, atk_up) }.invoke();
                });
        }

    };

    // 开拓者·火被动技能2：筑城者遗宝
    // 回合结束时我方全体获得4点护盾（2回合）。
    struct TailblazerFireP2 : public Trigger {
        int shield_value = 4 * 10;
        int shield_duration = 2;

        TailblazerFireP2(BattleUnit& owner) :
            Trigger(owner, TriggerTime::TurnEnd) {}

        const char* name() const override { return "筑城者遗宝"; }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& turn_end = dynamic_cast<const TurnEnd&>(procedure);
            return &turn_end.target == &target;
        }

        void on_trigger(Procedure& procedure) override {
            auto& turn_end = dynamic_cast<TurnEnd&>(procedure);
            turn_end.on_trigger(*this, [&] {
                for (auto& u : turn_end.battle.teammates(target)) {
                    if (u->alive) {
                        AddEffect{ turn_end.battle, target, *u, {}, 
                            std::make_unique<Shield>(shield_duration, *u, shield_value) }.invoke();
                    }
                }
                });
        }
    };

    // 开拓者·火被动技能3：守护者召令
    // 进入战斗时，附加20点护盾（1回合）。
    struct TailblazerFireP3 : public Trigger {
        int shield_value = 20 * 10;
        int shield_duration = 1;

        TailblazerFireP3(BattleUnit& owner) :
            Trigger(owner, TriggerTime::GameStart) {}

        const char* name() const override { return "守护者召令"; }

        void on_trigger(Procedure& procedure) override {
            GameStart& game_start = dynamic_cast<GameStart&>(procedure);
            game_start.on_trigger(*this, [&] {
                AddEffect{ game_start.battle, target, target, {},
                     std::make_unique<Shield>(shield_duration, target, shield_value) }.invoke();
                });
        }

    };

    // 开拓者·火被动技能4：永屹城垣的壁垒（6星）
    // 施放终结技后防御提升2点（限2次）。
    struct TailblazerFireS6 : public Trigger {
        int defense_up = 2;
        int limit = 2;
        int count = 0;

        TailblazerFireS6(BattleUnit& owner) :
            Trigger(owner, TriggerTime::SkillEnd) {}
        
        const char* name() const override { return "永屹城垣的壁垒"; }

        std::string cli_text(Battle& battle) const override {
            return std::format("【{}】的防御提高{}点。（已用次数：{}）\n",
                target.colored_name(), defense_up, (
                    limit == count ?
                    Painter::red(std::format("{}/{}", count, limit)) :
                    Painter::green(std::format("{}/{}", count, limit))
                    ));
        }

        bool is_triggrable(const Procedure& procedure) override {
            const auto& skill_end = dynamic_cast<const EmitSkill&>(procedure);
            return &skill_end.user == &target && count < limit && has_tag(skill_end.tags, Tag::UltimateSkill);
        }

        void on_trigger(Procedure& procedure) override {
            auto& skill_end = dynamic_cast<EmitSkill&>(procedure);
            skill_end.on_trigger(*this, [&] {
                target.defense += defense_up;
                count++;
                });
        }
    
    };
}