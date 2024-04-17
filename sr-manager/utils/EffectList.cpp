#include "EffectList.hpp"
#include "../battle/Battle.hpp"

namespace sr {
    void EffectList::on_turn_begin(Battle& battle, BattleUnit& unit) {
        // 因为触发过程中可能会添加或者删除效果，所以这里不能直接遍历
        std::vector<Effect*> triggered_effects;
        while (true) {
            // 直到没有尚未触发的效果为止
            bool has_untriggered_effect = false;
            for (int i = 0; i < size(); i++) {
                if (std::find(triggered_effects.begin(), triggered_effects.end(), (*this)[i].get()) == triggered_effects.end()) {
                    auto ptr = (*this)[i].get();
                    ptr->on_turn_begin(battle, unit);
                    if (battle.finished()) return;
                    triggered_effects.push_back(ptr);
                    has_untriggered_effect = true;
                }
            }
            if (!has_untriggered_effect) {
                break;
            }
        }
        // 清理所有已经结束的效果
        for (int i = 0; i < size(); i++) {
            if ((*this)[i]->duration <= 0) {
                erase(begin() + i);
                i--;
            }
        }
    }

    void EffectList::on_turn_end(Battle& battle, BattleUnit& unit) {
        // 因为触发过程中可能会添加或者删除效果，所以这里不能直接遍历
        std::vector<Effect*> triggered_effects;
        while (true) {
            // 直到没有尚未触发的效果为止
            bool has_untriggered_effect = false;
            for (int i = 0; i < size(); i++) {
                if (std::find(triggered_effects.begin(), triggered_effects.end(), (*this)[i].get()) == triggered_effects.end()) {
                    auto ptr = (*this)[i].get();
                    ptr->on_turn_end(battle, unit);
                    if (battle.finished()) return;
                    triggered_effects.push_back(ptr);
                    has_untriggered_effect = true;
                }
            }
            if (!has_untriggered_effect) {
                break;
            }
        }
        // 清理所有已经结束的效果
        for (int i = 0; i < size(); i++) {
            if ((*this)[i]->duration <= 0) {
                erase(begin() + i);
                i--;
            }
        }
    }
}
