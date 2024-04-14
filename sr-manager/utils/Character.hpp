#pragma once

#include <string>

namespace sr {

    class Battle;
    class BattleUnit;

    // 命途
    enum class Path {
          Destruction  // 毁灭
        , Harmony      // 同谐
        , Abundance    // 丰饶
        , Hunt         // 巡猎
        , Preservation // 存护
        , Erudition    // 智识
        , Nihility     // 虚无
    };

    // 属性
    enum class Type {
          Physical   // 物理
        , Fire       // 火
        , Ice        // 冰
        , Wind       // 风
        , Lightning  // 雷
        , Quantum    // 量子
        , Imaginary  // 虚数
    };

    std::string type_name(Type type) {
        switch (type) {
        case Type::Physical: return "物理";
        case Type::Fire: return "火";
        case Type::Ice: return "冰";
        case Type::Wind: return "风";
        case Type::Lightning: return "雷";
        case Type::Quantum: return "量子";
        case Type::Imaginary: return "虚数";
        default: return "未知";
        }
    }


    // 实现一个角色的抽象类
    // 用于生成一个新的角色，而不是一个角色的实例
    // 因此不包含当前生命值、韧性值和当前状态等属性
    class Character {
    public:
        virtual const char* name() const = 0;
        virtual Path path() const = 0;
        virtual Type type() const = 0;
        virtual int max_hp() const = 0;
        virtual int max_mp() const = 0;
        virtual int origin_mp() const = 0;
        virtual int max_sp() const = 0;
        virtual int origin_sp() const = 0;
        virtual int attack() const = 0;
        virtual int defense() const = 0;
        virtual int speed() const = 0;
        virtual int hit() const = 0;
        virtual int resist() const = 0;
        virtual int critical() const = 0;
        virtual int penetration() const = 0;
        virtual int ultimate_skill_priority() const = 0;
        virtual bool ultimate_skill_condition(Battle& battle, BattleUnit& unit) const = 0;
        virtual void ultimate(Battle& battle, BattleUnit& unit) const = 0;
        virtual void add_star(Battle& battle, BattleUnit& unit, int star) = 0;
        virtual void add_triggers(Battle& battle, BattleUnit& unit) const = 0;
        virtual void take_turn_ai(Battle& battle, BattleUnit& unit) const = 0;
    };

}