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
        , Extra // 附加伤害
        , Bleed // 裂伤
        , Burn // 灼烧
        , Shock // 触电
        , WindShear // 风化
        
    };

    // 标签的名称
    const char* tag_name(Tag tag) {
        switch (tag) {
            case Tag::NormalAttack: return "普攻";
            case Tag::BattleSkill: return "战技";
            case Tag::PassiveSkill: return "被动";
            case Tag::UltimateSkill: return "终结技";
            case Tag::AdditionalSkill: return "追加攻击";
            case Tag::Single: return "单体";
            case Tag::Spread: return "扩散";
            case Tag::Random: return "随机";
            case Tag::All: return "全体";
            case Tag::Break: return "击破";
            case Tag::DamageOverTime: return "持续伤害";
            case Tag::Extra: return "附加伤害";
            case Tag::Bleed: return "裂伤";
            case Tag::Burn: return "灼烧";
            case Tag::Shock: return "触电";
            case Tag::WindShear: return "风化";
        }
        return "未知";
    }

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