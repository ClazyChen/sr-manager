#pragma once

#include "../battle/Battle.hpp"
#include <string>
#include <functional>

namespace sr {

    class Battle;
    class BattleUnit;

    // 技能标签
    enum class Tag {
        // 包含所有类型的技能标签
        
        // 技能的基本类型
          NormalAttack // 普攻
        , BattleSkill // 战技
        , PassiveSkill // 被动
        , UltimateSkill // 终结技
        , AdditionalSkill // 追加攻击

        // 伤害方式
        , Single // 单体
        , Spread // 扩散
        , Random // 随机
        , All // 全体

        // 伤害类型
        , Break // 击破伤害
        , DamageOverTime // 持续伤害
        , Bleed // 裂伤
        , Burn // 灼伤
        , Shock // 电伤
        , WindShear // 风伤
        
    };

    bool has_tag(const std::vector<Tag>& tags, Tag tag) {
        return std::find(tags.begin(), tags.end(), tag) != tags.end();
    }

    // 实现一个技能类
    class Skill {
    protected:
        // 单体攻击技能的通用选择方法
        BattleUnit& target_ai_single(Battle& battle, BattleUnit& user, std::function<double(BattleUnit&)> score) const {
            // 选择最佳目标
            double best_score = -1;
            int best_target_hp = std::numeric_limits<int>::max();
            BattleUnit* best_target = nullptr;
            for (auto& u : battle.opponents(user)) {
                if (!u->alive) continue;
                double s = score(*u);
                if (s > best_score) {
                    best_score = s;
                    best_target = u.get();
                    best_target_hp = u->hp;
                } else if (s == best_score && u->hp < best_target_hp) {
                    best_target = u.get();
                    best_target_hp = u->hp;
                }
            }
            return *best_target;
        }

        // 扩散攻击技能的通用选择方法
        BattleUnit& target_ai_spread(Battle& battle, BattleUnit& user, 
            std::function<double(BattleUnit&)> score_main,
            std::function<double(BattleUnit&)> score_side) const {
            // 选择最佳目标
            double best_score = -1;
            int best_target_hp = std::numeric_limits<int>::max();
            BattleUnit* best_target = nullptr;
            for (auto& u : battle.opponents(user)) {
                if (!u->alive) continue;
                double s = score_main(*u);
                if (battle.left(*u)) {
                    s += score_side(*battle.left(*u));
                }
                if (battle.right(*u)) {
                    s += score_side(*battle.right(*u));
                }
                if (s > best_score) {
                    best_score = s;
                    best_target = u.get();
                    best_target_hp = u->hp;
                } else if (s == best_score && u->hp < best_target_hp) {
                    best_target = u.get();
                    best_target_hp = u->hp;
                }
            }
            return *best_target;
        }

    public:
        virtual const char* name() const = 0; // 技能名称
        virtual std::string description() const = 0; // 技能描述
        virtual std::vector<Tag> tags() const = 0; // 技能标签
        virtual BattleUnit& target_ai(Battle& battle, BattleUnit& user) const = 0; // 智能指定技能目标
        virtual void apply(Battle& battle, BattleUnit& user, BattleUnit& target) const = 0; // 技能效果
    };
}