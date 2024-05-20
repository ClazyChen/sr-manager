#pragma once

#include "physical/TailblazerPhysical/TailblazerPhysical.hpp"
#include "wind/DanHeng/DanHeng.hpp"
#include "Fire/Himeko/Himeko.hpp"
#include "Fire/Asta/Asta.hpp"
#include "Fire/TailblazerFire/TailblazerFire.hpp"
#include "imaginary/Welt/Welt.hpp"
#include "Ice/March7th/March7th.hpp"
#include "Ice/Herta/Herta.hpp"
#include "Lightning/Arlan/Arlan.hpp"
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
                emplace_back(std::make_unique<March7th>());
                emplace_back(std::make_unique<Herta>());
                emplace_back(std::make_unique<Arlan>());
                emplace_back(std::make_unique<Asta>());
                emplace_back(std::make_unique<TailblazerFire>());
            }        


        }

        Character& operator[](int index) {
            return *std::vector<std::unique_ptr<Character>>::operator[](index);
        }

    };

}