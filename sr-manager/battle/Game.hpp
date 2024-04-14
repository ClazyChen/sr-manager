#pragma once

#include "Battle.hpp"
#include "GameStart.hpp"
#include "TurnBegin.hpp"
#include "../utils/TriggerList.hpp"

namespace sr {
    // 完成从进入战斗开始整个游戏的过程
    // 战斗单位的初始化、安装和加入战场不在这个过程中
    struct Game {
        Battle& battle;     // 战场

        // 构造函数
        Game(Battle& battle) : battle(battle) {}

        void run() {
            // 初始化触发器（装载被动技能）
            battle.add_triggers();
            // 游戏开始时（触发一些被动技能）
            GameStart { battle }.invoke();
            // 初始化速度条
            battle.init_speed_bar();
            while (!battle.finished()) {
                // 游戏没有结束时，持续进行下一个回合
                battle.interface.print("\n");
                // 移动速度条
                battle.speed_bar.next_turn();
                // 输出速度条
                battle.interface.speed_bar(battle.speed_bar);
                // 输出战场
                battle.interface.battle_field(battle);
                // 等待用户输入任意键继续
                battle.interface.wait();
                // 进行回合
                TurnBegin { battle, battle.speed_bar.current() }.invoke();
                battle.speed_bar.current().take_turn(battle);
                // 插入回合（终结技）
                while (battle.insert_turn()) { }
                // 更新速度条
                battle.speed_bar.update();
            }
        }
    };
}