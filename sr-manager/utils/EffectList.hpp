#pragma once

#include "Effect.hpp"
#include <vector>
#include <memory>

namespace sr {

    struct EffectList : public std::vector<std::unique_ptr<Effect>> {

        // 添加效果
        void add(std::unique_ptr<Effect> effect) {
            push_back(std::move(effect));
        }

        // 存在效果
        bool has(EffectType type) {
            for (auto& effect : *this) {
                if (effect->type() == type) {
                    return true;
                }
            }
            return false;
        }

        // 回合开始时触发
        void on_turn_begin(Battle& battle, BattleUnit& unit) {
            // int sz = size();
            for (int i = 0; i < size(); i++) {
                (*this)[i]->on_turn_begin(battle, unit);
                if ((*this)[i]->duration <= 0) {
                    erase(begin() + i);
                    i--;
                    // sz--;
                }
            }
        }

        // 回合结束时触发
        void on_turn_end(Battle& battle, BattleUnit& unit) {
            // int sz = size();
            for (int i = 0; i < size(); i++) {
                (*this)[i]->on_turn_end(battle, unit);
                if ((*this)[i]->duration <= 0) {
                    erase(begin() + i);
                    i--;
                    // sz--;
                }
            }
        }

    };


}