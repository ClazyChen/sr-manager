#pragma once

#include <string>
#include <vector>
#include "../cli/Painter.hpp"
#include <format>

namespace sr {

    class BattleUnit;
    class Procedure;

    // 触发时机的枚举类
    enum class TriggerTime {
          GameStart // 游戏开始时
        , TurnBegin // 回合开始时
        , TurnEnd   // 回合结束时
        , Break     // 击破时
        , Kill      // 击杀时
        , Die       // 死亡时

        , AttackBegin // 攻击开始时（用于调整临时参数）
        , AttackEnd   // 攻击结束时
        , SkillBegin  // 技能开始时
        , SkillEnd    // 技能结束时
        , HpDownEnd   // 生命值减少结束时
    };

    // 施加在全局的触发器
    struct Trigger {

        // 触发器的触发对象
        BattleUnit& target;

        // 触发时机
        TriggerTime time;

        Trigger(BattleUnit& target, TriggerTime time)
            : target(target), time(time) {}

        virtual const char* name() const {  return ""; } // 名称
        virtual std::string description() const {  return ""; } // 描述
        virtual std::string cli_text(Battle& battle) const { return description(); } // 在 CLI 中显示的文本
        virtual bool is_triggrable(const Procedure& procedure) {  return true; } // 是否可以触发
        virtual void on_trigger(Procedure& procedure) = 0; // 触发效果
    };

}