#pragma once

#include "../../Utils.hpp"
#include "../../../battle/Attack.hpp"
#include "../../effects/Freeze.hpp"
#include "../../../battle/GetSkillPoint.hpp"
#include "../../../battle/CostSkillPoint.hpp"
#include "../../../battle/MpUp.hpp"
#include "../../../battle/MpDown.hpp"
#include "../../../battle/ReduceToughness.hpp"
#include "../../../battle/AddEffect.hpp"
#include "../../../battle/RemoveDebuff.hpp"
#include "../../effects/Taunt.hpp"
#include "../../effects/Shield.hpp"
#include "../../../cli/Painter.hpp"
#include "../../ai/Target.hpp"

namespace sr {

    // 三月七A：极寒的弓矢
    // 对敌方单体造成威力100的冰伤害，削韧2。
    struct March7thA : public Skill {
        int damage = 100;

        const char* name() const { return "极寒的弓矢"; }
        std::string description() const {
            return std::format(
                "对敌方单体造成威力{}的冰伤害，削韧2。",
                Painter::red(std::to_string(damage))
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::NormalAttack, Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            return target_ai_single(battle, user, [&](BattleUnit& u) {
                return ai::attack_expectation(Attack{
                    battle, user, u, tags(), damage, Type::Ice
                    });
                });
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 对目标造成伤害
            Attack{ battle, user, target, tags(), damage, Type::Ice }.invoke();
            // 削减目标 2 点韧性
            ReduceToughness{ battle, user, target, tags(), 2, Type::Ice }.invoke();
            // 获得 1 个战技点
            GetSkillPoint{ battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp{ battle, user, tags(), 1 }.invoke();
        }
    };

    // 三月七E：可爱即是正义
    // 单体附加36点护盾（4回合）并解除1个负面状态。当目标生命在50%以上时，嘲讽全体敌人（6命中·1回合）。
    struct March7thE : public Skill {
        int shield = 36;
        int shield_duration = 4;
        int taunt_hit = 6;
        int taunt_duration = 1;

        const char* name() const { return "可爱即是正义"; }
        std::string description() const {
            return std::format(
                "单体附加{}点护盾（{}回合）并解除1个负面状态。"
                "当目标生命在50%以上时，嘲讽全体敌人（{}命中·{}回合）。",
                Painter::cyan(std::to_string(shield)),
                shield_duration,
                taunt_hit, taunt_duration
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::Single };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            // 优先选择生命值最低且一半以下且没有护盾的友方单位
            int hp = std::numeric_limits<int>::max();
            BattleUnit* target = nullptr;
            for (auto& u : battle.teammates(user)) {
                if (u->alive && u->hp < hp && u->hp < u->max_hp / 2 && shield_value(*u) == 0) {
                    hp = u->hp;
                    target = u.get();
                }
            }
            if (target == nullptr) {
                int def = std::numeric_limits<int>::min();
                // 其次选择防御最高的没有护盾的友方单位
                for (auto& u : battle.teammates(user)) {
                    if (u->alive && u->defense > def && shield_value(*u) == 0) {
                        def = u->defense;
                        target = u.get();
                    }
                }
            }
            if (target == nullptr) {
                // 最后选择自己
                target = &user;
            }
            return *target;
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            // 附加护盾
            AddEffect { battle, user, target, tags(), std::make_unique<Shield>(shield_duration, user, shield * 10) }.invoke();
            // 解除 1 个负面状态
            RemoveDebuff { battle, user, target, tags() }.invoke();
            // 当目标生命在 50% 以上时，嘲讽全体敌人
            if (target.hp >= target.max_hp / 2) {
                for (auto& u : battle.opponents(user)) {
                    if (u->alive) {
                        AddEffect { battle, target, *u, tags(), std::make_unique<Taunt>(taunt_duration, target), taunt_hit }.invoke();
                    }
                }
            }
            // 消耗 1 个战技点
            CostSkillPoint { battle, user, tags(), 1 }.invoke();
            // 回复 1 点能量
            MpUp { battle, user, tags(), 1 }.invoke();
        }
    };

    // 三月七Q：冰刻箭雨之时
    // 消耗3点能量，对敌方全体造成威力150的冰伤害，削韧4，附加威力65的冻结状态（4命中·1回合）。
    struct March7thQ : public Skill {
        int damage = 150;
        int freeze = 65;
        int freeze_hit = 4;
        int freeze_duration = 1;
        int mp_back = 0;

        const char* name() const { return "冰刻箭雨之时"; }
        std::string description() const {
            return std::format(
                "消耗3点能量，对敌方全体造成威力{}的冰伤害，削韧4，附加威力{}的冻结状态（{}命中·{}回合）。{}",
                Painter::red(std::to_string(damage)),
                Painter::red(std::to_string(freeze)),
                freeze_hit, freeze_duration,
                mp_back > 0 ? std::format("如果至少冻结了1个目标，回复{}点能量。", mp_back) : ""
            );
        }

        std::vector<Tag> tags() const {
            return { Tag::UltimateSkill, Tag::All };
        }

        BattleUnit& target_ai(Battle& battle, BattleUnit& user) const {
            // 全体攻击，随便选择一个目标即可
            return *battle.random_target(battle.opponents(user));
        }

        void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const {
            MpDown{ battle, user, tags(), 3 }.invoke();
            // 对敌方全体造成伤害
            bool is_hit = false;
            for (auto& u : battle.opponents(user)) {
                if (u->alive) {
                    Attack{ battle, user, *u, tags(), damage, Type::Ice }.invoke();
                    // 削减目标 4 点韧性
                    ReduceToughness{ battle, user, *u, tags(), 4, Type::Ice }.invoke();
                    // 附加冻结状态
                    auto add_effect = AddEffect{ battle, user, *u, tags(), std::make_unique<Freeze>(freeze_duration, user, freeze), freeze_hit };
                    add_effect.invoke();
                    if (add_effect.is_hit) {
                        is_hit = true;
                    }
                }
            }
            // 如果至少冻结了 1 个目标，回复 1 点能量
            if (is_hit && mp_back > 0) {
                MpUp{ battle, user, tags(), mp_back }.invoke();
            }
        }
    };
}
