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

        , Else        // 其他
    };

    // 一种施加在单位身上的持续效果的基类
    struct Effect {
        // 持续时间
        int duration;

        // 构造函数
        Effect(int duration) : duration(duration) {}

        virtual EffectType type() const = 0;
        virtual std::string name() const = 0;
        virtual std::string colored_name() const {
            return name();
        }
        virtual std::string description() const = 0;
        // 在施加时触发的效果
        virtual void on_apply(Battle& battle, BattleUnit& unit) {}
        // 在回合开始时结束的效果
        virtual void on_turn_begin(Battle& battle, BattleUnit& unit) {}
        // 在回合结束时结束的效果
        virtual void on_turn_end(Battle& battle, BattleUnit& unit) {}
    };

}