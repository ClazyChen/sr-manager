#pragma once

#include "../utils/CharacterData.hpp"
#include "BattleCli.hpp"
#include "../battle/Game.hpp"

#include <iostream>

namespace sr {

    using namespace std::string_literals;

    // 创建战场的命令行界面
    struct CreatorCli {

        int max_unit = 1;

        CharacterData data;
        BattleCli battle_cli;
        Battle battle;
        Game game;

        CreatorCli() : battle(battle_cli), game(battle) {}

        void select_character(std::vector<std::unique_ptr<BattleUnit>>& vec, Side side) {
            int index, star;
            for (auto i = 0; i < data.size(); i++) {
                std::cout << Painter::white(std::format("({}) {}", i, data[i].name())) << std::endl;
            }
            while (true) {
                std::cin >> index >> star;
                if (index < 0 || index >= data.size()) {
                    std::cout << "输入序号不合法，请重新输入" << std::endl;
                    continue;
                }
                if (star < 0 || star > 6) {
                    std::cout << "输入星级不合法，请重新输入" << std::endl;
                    continue;
                }
                break;
            }
            std::cout << Painter::yellow(std::format("{}（{}★）已被选择", data[index].name(), star)) << std::endl;
            auto unit = std::make_unique<BattleUnit>(data[index]);
            unit->debut(side, vec.size(), star);
            data[index].add_star(battle, *unit, star);
            vec.push_back(std::move(unit));
        }

        void run() {
            std::cout << "测试创建者界面" << std::endl;
            std::cout << "输入最大单位数：";
            std::cin >> max_unit;
            std::cout << "蓝方角色选择（输入序号 星级）：" << std::endl;
            for (int i = 0; i < max_unit; i++) {
                select_character(battle.blue, Side::Blue);
            }
            std::cout << "红方角色选择（输入序号 星级）：" << std::endl;
            for (int i = 0; i < max_unit; i++) {
                select_character(battle.red, Side::Red);
            }
            game.run();
        }

    };

}