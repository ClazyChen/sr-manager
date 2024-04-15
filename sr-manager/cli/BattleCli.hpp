#pragma once

#include "../battle/Battle.hpp"
#include "HpBar.hpp"
#include <iostream>
#include <ranges>

namespace sr {
    // 进入战场之后的交互界面
    struct BattleCli {
        bool print_battle_field = true; // 是否打印战场信息
        bool print_procedure = true; // 是否打印战斗过程
        bool print_speed_bar = true; // 是否打印速度条
        bool wait_every_step = true; // 每一步等待用户输入任意键继续
    private:
        static std::string blocks(int n) {
            std::string res = "";
            for (int i = 0; i < n; ++i) {
                res += "■";
            }
            return res;
        }
    public:
        std::ostream& os;
        BattleCli(std::ostream& os = std::cout) : os(os) {}

        void hide_all() {
            print_battle_field = false;
            print_procedure = false;
            print_speed_bar = false;
            wait_every_step = false;
        }
        // 打印一个战场单位的信息
        void battle_unit(const BattleUnit& unit) const {
            if (unit.side == Side::Blue) {
                os << Painter::cyan(unit.name());
            } else {
                os << Painter::red(unit.name());
            }
            if (unit.alive) {
                os << " " << HpBar::draw(unit.hp) << " ";
                os << std::format("{}.{} / {}.0\n", unit.hp / 10, unit.hp % 10, unit.max_hp / 10);
            } else {
                os << " " << Painter::red("已阵亡") << "\n";
            }
            os << "  韧性 " << Painter::purple(blocks(unit.toughness)) << Painter::white(blocks(unit.max_toughness - unit.toughness));
            os << "  能量 " << Painter::yellow(blocks(unit.mp)) << Painter::white(blocks(unit.max_mp - unit.mp));
            if (unit.max_sp > 0) {
                os << "  技力 " << Painter::green(blocks(unit.sp)) << Painter::white(blocks(unit.max_sp - unit.sp));
            } else {
                os << "  ";
            }
            for (auto& effect : unit.effects) {
                os << "【" << effect->colored_name() << effect->duration << "】";
            }
            os << "\n";
        }

        // 打印战场信息
        void battle_field(const Battle& battle) const {
            if (!print_battle_field) return;
            os << Painter::cyan("蓝方战技点 ");
            for (int i = 0; i < Battle::MAX_SKILL_POINTS; i++) {
                if (i < battle.blue_skill_points) {
                    os << Painter::cyan("★ ");
                } else {
                    os << "☆ ";
                }
            }
            os << "\n";
            for (const auto& unit : battle.blue) {
                battle_unit(*unit);
            }
            os << Painter::red("红方战技点 ");
            for (int i = 0; i < Battle::MAX_SKILL_POINTS; i++) {
                if (i < battle.red_skill_points) {
                    os << Painter::red("★ ");
                } else {
                    os << "☆ ";
                }
            }
            os << "\n";
            for (const auto& unit : battle.red) {
                battle_unit(*unit);
            }
        }

        // 打印速度条
        void speed_bar(const SpeedBar& bar) const {
            if (!print_speed_bar) return;
            // 打印第1个元素
            os << std::format("【{}】的回合\n", bar.nodes.front().unit->colored_name());
            os << "速度条 ";
            // 从第2个元素开始打印
            for (const auto& node : bar.nodes | std::views::drop(1)) {
                if (node.unit->side == Side::Blue) {
                    os << Painter::cyan(node.unit->name());
                } else if (node.unit->side == Side::Red) {
                    os << Painter::red(node.unit->name());
                } else {
                    os << node.unit->name(); // 无阵营（召唤物等）
                }
                os << std::format("【{:0.1f}】", node.time_to_act - bar.global_time);
            }
            os << "\n";
        }

        // 打印战斗过程
        void print(std::string str) const {
            if (!print_procedure) return;
            if (str.empty()) return;
            os << str << "\n";
        }

        void print_raw(std::string str) const {
            if (!print_procedure) return;
            os << str;
        }

        // 等待用户输入任意键继续
        void wait() const {
            if (!wait_every_step) return;
            getchar();
        }
    };

}