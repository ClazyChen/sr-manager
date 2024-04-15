#pragma once

#include "physical/TailblazerPhysical/TailblazerPhysical.hpp"
#include "wind/DanHeng/DanHeng.hpp"
#include "Fire/Himeko/Himeko.hpp"
#include "imaginary/Welt/Welt.hpp"
#include <vector>
#include <memory>

namespace sr {

    // 角色数据库，包含所有的角色
    struct CharacterData : public std::vector<std::unique_ptr<Character>>{

        CharacterData( bool init = true) {
            if (init) {
                // 创建所有角色的实例
                emplace_back(std::make_unique<TailblazerPhysical>());
                emplace_back(std::make_unique<DanHeng>());
                emplace_back(std::make_unique<Himeko>());
                emplace_back(std::make_unique<Welt>());
            }        


        }

        Character& operator[](int index) {
            return *std::vector<std::unique_ptr<Character>>::operator[](index);
        }

    };

}