#pragma once

#include "Painter.hpp"
#include <string>

namespace sr {

    class HpBar {
        static constexpr int HP_BAR_WIDTH = 40;

    public:
        static auto draw(int hp) {
            std::string bar = "";
            while (hp >= HP_BAR_WIDTH) {
                bar += "█";
                hp -= HP_BAR_WIDTH;
            }
            if (hp > HP_BAR_WIDTH / 2) {
                bar += "▌";
            }
            return Painter::red(bar);
        }
    };

}

