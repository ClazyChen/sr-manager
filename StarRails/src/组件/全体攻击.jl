# 全体攻击的模板

# 选择全体攻击的目标，返回目标和预期伤害
function 选择全体攻击的目标(战场::战场, 来源::角色, 威力::Int)
    预期伤害 = 0.0
    目标列表 = 敌方队伍(战场, 来源)
    for 目标 in 目标列表
        预期伤害 += 攻击预期伤害(来源, 目标, 威力)
    end
    return 目标列表 => 预期伤害
end

macro 全体攻击()
    quote
        @目标 全体攻击
        @效果 所有目标(技能) begin
            攻击并削韧!(事件, 目标 = 目标, 威力 = 技能.威力, 削韧 = 技能.削韧)
        end
    end
end