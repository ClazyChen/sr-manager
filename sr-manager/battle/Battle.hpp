#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "BattleUnit.hpp"
#include "SpeedBar.hpp"
#include "../utils/Random.hpp"
#include "../utils/TriggerList.hpp"

namespace sr {

    struct BattleCli;

    // 实现一个战场类，创造一个战场
    struct Battle {
        static constexpr int MAX_SKILL_POINTS = 5; // 最大技能点数
        static constexpr int ORIGINAL_SKILL_POINTS = 3; // 初始技能点数

        std::vector<std::unique_ptr<BattleUnit>> blue; // 蓝方
        std::vector<std::unique_ptr<BattleUnit>> red;  // 红方

        Winner winner = Winner::Unfinished; // 胜利者
        SpeedBar speed_bar; // 速度条
        BattleCli& interface; // 交互界面
        TriggerList triggers; // 触发器列表
        int blue_skill_points = ORIGINAL_SKILL_POINTS; // 蓝方技能点数
        int red_skill_points = ORIGINAL_SKILL_POINTS; // 红方技能点数

        // 构造函数
        Battle(BattleCli& interface) : interface(interface), triggers(*this) {}

        // 初始化速度条
        void init_speed_bar() {
            speed_bar.nodes.clear();
            for (auto& unit : blue) {
                speed_bar.insert(*unit, unit->time_cost());
            }
            for (auto& unit : red) {
                speed_bar.insert(*unit, unit->time_cost());
            }
        }

        int& skill_points(Side side) {
            return side == Side::Blue ? blue_skill_points : red_skill_points;
        }
        auto& teammates(const BattleUnit& unit) {
            return unit.side == Side::Blue ? blue : red;
        }
        auto& opponents(const BattleUnit& unit) {
            return unit.side == Side::Blue ? red : blue;
        }
        auto& random_target( std::vector<std::unique_ptr<BattleUnit>>& vec) {
            int index = 0;
            do {
                index = Random::get(0, vec.size() - 1);
            } while (!vec[index]->alive);
            return vec[index];
        }

        bool finished() const {
            return winner != Winner::Unfinished;
        }
        void judge_success() {
            // 判断胜负
            bool blue_alive = std::any_of(blue.begin(), blue.end(), [](const auto& unit) { return unit->alive; });
            bool red_alive = std::any_of(red.begin(), red.end(), [](const auto& unit) { return unit->alive; });
            if (!blue_alive) {
                // 蓝方全灭，红方胜利
                winner = Winner::Red;
            } else if (!red_alive) {
                // 红方全灭，蓝方胜利
                winner = Winner::Blue;
            }
        }

        // 一个角色左侧的角色
        BattleUnit* left(BattleUnit& unit) {
            if (unit.position == 0) return nullptr;
            auto& units = unit.side == Side::Blue ? blue : red;
            for (auto i = unit.position - 1; i >= 0; --i) {
                if (units[i]->alive) return units[i].get();
            }
            return nullptr;
        }

        // 一个角色右侧的角色
        BattleUnit* right(BattleUnit& unit) {
            auto& units = unit.side == Side::Blue ? blue : red;
            if (unit.position == units.size() - 1) return nullptr;
            for (auto i = unit.position + 1; i < units.size(); ++i) {
                if (units[i]->alive) return units[i].get();
            }
            return nullptr;
        }

        // 插入回合（终结技）
        bool insert_turn() {
            int max_priority = 10;
            BattleUnit* unit = nullptr;
            for (auto& u : teammates(speed_bar.current())) {
                if (u->alive && u->ultimate_skill_condition(*this) && u->ultimate_skill_priority() < max_priority) {
                    unit = u.get();
                    max_priority = u->ultimate_skill_priority();
                }
            }
            if (unit) {
                // 如果有终结技可以释放
                unit->character.ultimate(*this, *unit);
            }
            return unit != nullptr;
        }

        // 安装触发器，按照蓝1、红1、蓝2、红2、...的顺序
        void add_triggers() {
            for (int i = 0; i < std::max(blue.size(), red.size()); i++) {
                if (i < blue.size()) {
                    blue[i]->character.add_triggers(*this, *blue[i]);
                }
                if (i < red.size()) {
                    red[i]->character.add_triggers(*this, *red[i]);
                }
            }
        }

        // 触发触发器
        void invoke(TriggerTime time, Procedure& procedure) {
            for (auto& trigger : triggers) {
                if (finished()) return;
                if (trigger->time == time && trigger->target.alive && trigger->is_triggrable(procedure)) {
                    trigger->on_trigger(procedure);
                }
            }
        }
    };

}