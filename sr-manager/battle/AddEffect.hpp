#pragma once

#include "Procedure.hpp"
#include "../utils/effects/WindShear.hpp"
#include "../utils/effects/Taunt.hpp"

namespace sr {

    // 实现添加效果这一基本过程
    struct AddEffect : public Procedure {
        BattleUnit& from;        // 施加者
        BattleUnit& target;      // 目标
        std::unique_ptr<Effect> effect;  // 效果
        int hit_rate;            // 命中率
        bool is_hit;             // 是否命中

        // 构造函数
        AddEffect(Battle& battle, BattleUnit& from, BattleUnit& target, std::vector<Tag> tags, std::unique_ptr<Effect> effect, int hit_rate = 100)
            : Procedure(battle, tags), from(from), target(target), effect(std::move(effect)), hit_rate(hit_rate) {}

        // 执行添加效果的效果
        void apply() override {
            if (!target.alive) return;
            auto actual_hit_rate = hit_rate + from.hit - target.resist;
            is_hit = Random::dice(actual_hit_rate);
            auto name = effect->colored_name();
            if (is_hit) {
                // 特殊判定：如果是风化状态，且目标已经有风化状态，会刷新已有状态的持续时间
                if (effect->type() == EffectType::WindShear) {
                    auto& wind_shear = dynamic_cast<WindShear&>(*effect);
                    for (auto& e : target.effects) {
                        if (e->type() == EffectType::WindShear) {
                            auto& target_wind_shear = dynamic_cast<WindShear&>(*e);
                            target_wind_shear.duration = std::max(target_wind_shear.duration, wind_shear.duration);
                        }
                    }
                }
                // 特殊判定：如果是嘲讽状态，且目标已经有嘲讽状态，则会移除已有的嘲讽状态
                if (effect->type() == EffectType::Taunt) {
                    for (auto it = target.effects.begin(); it != target.effects.end(); ) {
                        if ((*it)->type() == EffectType::Taunt) {
                            it = target.effects.erase(it);
                        } else {
                            ++it;
                        }
                    }
                }
                // 效果生效
                effect->on_apply(battle, target);
                // 添加效果
                target.effects.add(std::move(effect));
            }
            battle.interface.print(std::format("【{}】对【{}】施加效果【{}】（{}）。"
                , from.colored_name(), target.colored_name(), name, 
                is_hit ? Painter::green("成功") : Painter::red("抵抗") 
                ));
        }

    };

}