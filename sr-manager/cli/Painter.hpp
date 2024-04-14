#pragma once

#include <format>
#include <map>

namespace sr {

    // 基本颜色的枚举
    enum class Color {
        Black, Red, Green, Yellow, Blue, Purple, Cyan, White
    };

    // 为了方便输出带颜色的字符串
    class Painter {

        static inline const std::map<Color, char> color_code = {
            { Color::Black, '0'},
            { Color::Red, '1'},
            { Color::Green, '2'},
            { Color::Yellow, '3'},
            { Color::Blue, '4'},
            { Color::Purple, '5'},
            { Color::Cyan, '6'},
            { Color::White, '7'}
        };

        template <Color color>
        static std::string colorize(const std::string& str) {
            return std::format("\033[3{}m{}\033[0m", color_code.at(color), str);
        }

    public:
        static auto black(const std::string& str) { return colorize<Color::Black>(str); }
        static auto red(const std::string& str) { return colorize<Color::Red>(str); }
        static auto green(const std::string& str) { return colorize<Color::Green>(str); }
        static auto yellow(const std::string& str) { return colorize<Color::Yellow>(str); }
        static auto blue(const std::string& str) { return colorize<Color::Blue>(str); }
        static auto purple(const std::string& str) { return colorize<Color::Purple>(str); }
        static auto cyan(const std::string& str) { return colorize<Color::Cyan>(str); }
        static auto white(const std::string& str) { return colorize<Color::White>(str); }
    };

}