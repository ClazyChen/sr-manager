#pragma once

#include <string>

namespace sr {

    class Battle;
    class BattleUnit;

    // 持续效果类型枚举
    enum class EffectType {
        Bleed       // 裂伤
        , WindShear   // 风化
        , Burn        // 灼烧
        , Freeze      // 冻结
        , Shock       // 触电
        , Entanglement // 纠缠
        , Imprisonment // 禁锢
        , Slow        // 减速
        , Fast        // 加速
        , ResistDown  // 抗性降低
        , AttackUpgrade // 攻击强效
        , Taunt       // 嘲讽
        , Shield      // 护盾
        , Unyielding  // 不屈

        , Else        // 其他
    };

    inline std::string type_name(EffectType type) {
        switch (type) {
        case EffectType::Bleed: return "裂伤";
        case EffectType::WindShear: return "风化";
        case EffectType::Burn: return "灼烧";
        case EffectType::Freeze: return "冻结";
        case EffectType::Shock: return "触电";
        case EffectType::Entanglement: return "纠缠";
        case EffectType::Imprisonment: return "禁锢";
        case EffectType::Slow: return "减速";
        case EffectType::Fast: return "加速";
        case EffectType::ResistDown: return "抗性降低";
        case EffectType::AttackUpgrade: return "攻击强效";
        case EffectType::Taunt: return "嘲讽";
        case EffectType::Shield: return "护盾";
        case EffectType::Unyielding: return "不屈";
        case EffectType::Else: return "其他";
        default: return "未知";
        }
    }

    // 持续效果性质
    enum class EffectAttr {
        Positive    // 正面效果
        , Negative    // 负面效果
        , Neutral     // 中性效果
    };

    // 一种施加在单位身上的持续效果的基类
    struct Effect {
        // 持续时间
        int duration;

        // 构造函数
        Effect(int duration) : duration(duration) {}

        virtual EffectType type() const = 0;
        virtual EffectAttr attr() const { return EffectAttr::Neutral; }
        virtual std::string name() const = 0;
        virtual std::string colored_name() const {
            return name();
        }
        virtual std::string description() const = 0;
        // 在施加时触发的效果
        virtual void on_apply(Battle& battle, BattleUnit& unit) {}
        // 在移除时触发的效果
        virtual void on_remove(Battle& battle, BattleUnit& unit) {}
        // 在回合开始时结束的效果
        virtual void on_turn_begin(Battle& battle, BattleUnit& unit) {}
        // 在回合结束时结束的效果
        virtual void on_turn_end(Battle& battle, BattleUnit& unit) {}
    };

}