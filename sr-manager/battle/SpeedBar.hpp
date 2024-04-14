#pragma once

#include "BattleUnit.hpp"
#include <vector>

namespace sr {

    // 速度条，战场的重要组成元件，用于决定行动顺序
    struct SpeedBar {

        float global_time = 0; // 全局时间
        
        struct Node {
            BattleUnit* unit; // 战斗单位
            float time_to_act;  // 下次行动的时间
            bool extra;       // 是否是额外行动，非额外行动在行动之后会重新计算时间
            bool operator<(const Node& other) const {
                if (time_to_act == other.time_to_act) {
                    return unit->order() < other.unit->order();
                }
                return time_to_act < other.time_to_act;
            }
        };

        std::vector<Node> nodes; // 速度条上的节点（递增）

        // 插入一个战斗单位
        void insert(BattleUnit& unit, float time_to_act) {
            Node node { &unit, time_to_act };
            auto it = std::upper_bound(nodes.begin(), nodes.end(), node);
            nodes.insert(it, std::move(node));
        }

        // 删除一个战斗单位（及其相关的召唤物）
        void remove(BattleUnit& unit) {
            nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [&unit](const Node& node) {
                return node.unit == &unit || node.unit->summoner == &unit;
            }), nodes.end());
        }

        // 进入下一个回合
        void next_turn() {
            global_time = nodes.front().time_to_act;
        }

        // 更新速度条（当前行动的战斗单位行动完毕）
        void update() {
            auto node = nodes.front();
            nodes.erase(nodes.begin());
            if (!node.extra) {
                auto next_time = node.unit->time_cost() + global_time;
                insert(*node.unit, next_time);
            }
        }

        // 当前行动的战斗单位
        BattleUnit& current() {
            return *nodes.front().unit;
        }

        // 加速或者减速
        void move(BattleUnit& unit, int cost_before, int cost_after) {
            auto it = std::find_if(nodes.begin(), nodes.end(), [&unit](const Node& node) {
                return node.unit == &unit;
            });
            if (it == nodes.end() || it == nodes.begin()) {
                return;
            }
            auto node = *it;
            nodes.erase(it);
            node.time_to_act += ( node.time_to_act - global_time ) * ( cost_after - cost_before ) / cost_before;
            insert(*node.unit, node.time_to_act);
        }

        // 插入一个额外的回合
        void insert_extra(BattleUnit& unit) {
            insert(unit, global_time);
            nodes.front().extra = true;
        }

    };

}