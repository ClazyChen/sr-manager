# 随机攻击的模板

function 选择随机攻击的目标(战场::战场, 来源::角色, 威力::Int, 次数::Int)
    预期伤害 = 0.0
    目标列表 = 敌方队伍(战场, 来源)
    for 目标 in 目标列表
        预期伤害 += 攻击预期伤害(来源, 目标, 威力)
    end
    预期伤害 = 预期伤害 * 次数 / length(目标列表)
    return 目标列表 => 预期伤害
end

# 确实和全体攻击一样，这是因为在指定目标的时候就已经进行随机化了
macro 随机攻击()
    quote
        @目标 随机攻击
        @效果 所有目标(技能) begin
            攻击并削韧!(事件, 目标 = 目标, 威力 = 技能.威力, 削韧 = 技能.随机目标削韧)
        end
    end
end