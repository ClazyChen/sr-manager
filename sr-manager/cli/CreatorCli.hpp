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

        // 用户输入的序号和星级缓存，用于重复进行
        std::vector<std::pair<int, int>> data_temp;

        CreatorCli() : battle(battle_cli), game(battle) {}

        void initialize(CreatorCli& creator) {
            creator.max_unit = max_unit;
            for (int i = 0; i < max_unit; i++) {
                auto& [index, star] = data_temp[i];
                auto unit = std::make_unique<BattleUnit>(data[index]);
                unit->debut(Side::Blue, creator.battle.blue.size(), star);
                data[index].add_star(creator.battle, *unit, star);
                creator.battle.blue.push_back(std::move(unit));
            }
            for (int i = 0; i < max_unit; i++) {
                auto& [index, star] = data_temp[i + max_unit];
                auto unit = std::make_unique<BattleUnit>(data[index]);
                unit->debut(Side::Red, creator.battle.red.size(), star);
                data[index].add_star(creator.battle, *unit, star);
                creator.battle.red.push_back(std::move(unit));
            }
        }

        void select_character(std::vector<std::unique_ptr<BattleUnit>>& vec, Side side, int max_unit) {
            int index, star;
            for (auto i = 0; i < data.size(); i++) {
                std::cout << Painter::white(std::format("({}) {}", i, data[i].name())) << std::endl;
            }
            std::cout << Painter::white(std::format("({}) 随机选择", data.size())) << std::endl;
            std::vector<int> temp;
            for (int i = 0; i < max_unit; i++) {
                
                while (true) {
                    star = 0;
                    std::cin >> index;// >> star;
                    if (index < 0 || index >= data.size() + 1) {
                        std::cout << "输入序号不合法，请重新输入" << std::endl;
                        continue;
                    }
                    if (star < 0 || star > 6) {
                        std::cout << "输入星级不合法，请重新输入" << std::endl;
                        continue;
                    }
                    if (index == data.size()) {
                        do {
                            index = Random::get(0, data.size() - 1);
                        } while (std::find(temp.begin(), temp.end(), index) != temp.end());
                    }
                    temp.push_back(index);
                    break;
                }
                std::cout << Painter::yellow(std::format("{}（{}★）已被选择", data[index].name(), star)) << std::endl;
                auto unit = std::make_unique<BattleUnit>(data[index]);
                unit->debut(side, vec.size(), star);
                data[index].add_star(battle, *unit, star);
                vec.push_back(std::move(unit));
                data_temp.push_back({ index, star });
            }
        }

        void run() {
            std::cout << "测试创建者界面" << std::endl;
            std::cout << "输入最大单位数：";
            std::cin >> max_unit;
            std::cout << "蓝方角色选择（输入序号 星级）：" << std::endl;
            select_character(battle.blue, Side::Blue, max_unit);
            std::cout << "红方角色选择（输入序号 星级）：" << std::endl;
            select_character(battle.red, Side::Red, max_unit);
            std::cout << "模拟模式选择：" << std::endl;
            std::cout << "(0) 单次模拟，观看战斗过程" << std::endl;
            std::cout << "(N) N次模拟，观看胜率" << std::endl;
            int mode;
            std::cin >> mode;
            if (mode == 0) {
                game.run();
            }
            else {
                int blue_win = 0;
                int red_win = 0;
                for (int i = 0; i < mode; i++) {
                    CreatorCli creator;
                    initialize(creator);
                    creator.battle_cli.hide_all();
                    auto winner = creator.game.run();
                    if (winner == Winner::Blue) {
                        blue_win++;
                    }
                    else {
                        red_win++;
                    }
                }
                std::cout << std::format("蓝方获胜：{}次（{:0.1f}%）\n红方获胜：{}次（{:0.1f}%）", blue_win, blue_win * 100.0 / mode, red_win, red_win * 100.0 / mode) << std::endl;
            }
            
        }

    };

}