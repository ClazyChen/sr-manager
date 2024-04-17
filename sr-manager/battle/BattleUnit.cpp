#include "BattleUnit.hpp"
#include "Battle.hpp"

namespace sr {
    void BattleUnit::take_turn(Battle& battle) {

        // 如果韧性为 0，恢复韧性
        if (toughness == 0) {
            toughness = max_toughness;
        }
        effects.on_turn_begin(battle, *this);
        if (battle.finished()) return;
        character.take_turn_ai(battle, *this);
        if (battle.finished()) return;
        effects.on_turn_end(battle, *this);
    }

}