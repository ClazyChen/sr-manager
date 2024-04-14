#pragma once

#include "../utils/EffectList.hpp"
#include "../utils/Character.hpp"
#include "../cli/Painter.hpp"
#include <vector>
#include <iostream>

namespace sr {
    enum class Side {
        Blue, // 蓝方
        Red,  // 红方
        Neutral, // 中立（召唤物）
    };

    enum class Winner {
        Unfinished, // 未结束
        Blue, // 蓝方胜利
        Red,  // 红方胜利
    };

    // 创建一个战斗单位的类
    // 其基于一个角色的实例，包含了当前生命值、韧性值和当前状态等属性
    class BattleUnit {
        static constexpr int DEFAULT_TOUGHNESS = 10;
        static constexpr int SPEED_LIMIT = 20;

    public:
        // 入场前初始化的字段
        Character& character;
        const char* name() const { return character.name(); }
        int hp; // 生命值总是保留一位小数，这里存储的是生命值*10
        int max_hp; // 最大生命值，总是保留一位小数，这里存储的是生命值*10
        int mp; // 能量值
        int max_mp; // 最大能量值
        int sp; // 技力值
        int max_sp; // 最大技力值
        int toughness; // 韧性值
        int max_toughness; // 最大韧性值
        int attack; // 攻击力
        int defense; // 防御力
        int speed; // 速度
        int hit; // 效果命中
        int resist; // 效果抵抗
        int critical; // 暴击
        int penetration; // 穿甲
        bool alive = true; // 是否存活
        EffectList effects; // 效果列表
        BattleUnit(Character& character) : character(character) {
            this->hp = character.max_hp() * 10;
            this->max_hp = character.max_hp() * 10;
            this->mp = character.origin_mp();
            this->max_mp = character.max_mp();
            this->sp = character.origin_sp();
            this->max_sp = character.max_sp();
            this->toughness = DEFAULT_TOUGHNESS;
            this->max_toughness = DEFAULT_TOUGHNESS;
            this->attack = character.attack();
            this->defense = character.defense();
            this->speed = character.speed();
            this->hit = character.hit();
            this->resist = character.resist();
            this->critical = character.critical();
            this->penetration = character.penetration();
            this->alive = true;
            effects.clear();
        }
        int time_cost() const {
            return SPEED_LIMIT - speed;
        }
        int ultimate_skill_priority() const {
            return character.ultimate_skill_priority();
        }
        bool ultimate_skill_condition(Battle& battle) {
            return character.ultimate_skill_condition(battle, *this);
        }

        // 入场时初始化的字段
        Side side; // 阵营
        int position; // 位置
        int star; // 星级
        void debut(Side side, int position, int star) {
            this->side = side;
            this->position = position;
            this->star = star;
        }

        // 根据阵营和位置决定出手顺序
        // 蓝1、红1、蓝2、红2、……、蓝4、红4，召唤物在最后
        int order() const {
            if (side == Side::Neutral) return 100 + position;
            return (side == Side::Blue ? 0 : 1) + position * 2;
        }

        // 存在场上的召唤物（用于实现神君，账账）
        std::vector<BattleUnit*> summons;
        BattleUnit* summoner = nullptr;

        // 每个回合的行动 AI
        void take_turn(Battle& battle) {
            // 如果韧性为 0，恢复韧性
            if (toughness == 0) {
                toughness = max_toughness;
            }
            effects.on_turn_begin(battle, *this);
            character.take_turn_ai(battle, *this);
            effects.on_turn_end(battle, *this);
        }

        // 带颜色的名字
        std::string colored_name() const {
            if (side == Side::Blue) {
                return Painter::cyan(name());
            } else {
                return Painter::red(name());
            }
        }
    };

}