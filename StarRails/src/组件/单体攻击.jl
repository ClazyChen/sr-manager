# 单体攻击的模板

# 默认是根据预期伤害决定
# 根据威力计算预期伤害
function 攻击预期伤害(攻击者::角色, 防御者::角色, 威力::Int)::Float64
    local 实际攻击 = 攻击者.攻击 * 威力 / 10.0
    local 穿甲效果 = min(1.0, 攻击者.穿甲 / 6.0)
    local 基础伤害 = 实际攻击
    if 防御者.防御 > 0
        local 基础伤害 -= 防御者.防御 * 10 * (1 - 穿甲效果)
    else
        local 基础伤害 -= 防御者.防御 * 10
    end
    local 暴击效果 = min(1.0, 攻击者.暴击 / 6.0)
    基础伤害 * (1 + 暴击效果 / 2)
end

function 根据期望函数选择目标(战场::战场, 来源::角色, 期望函数::Function)
    当前最优的目标 = nothing
    当前最优的伤害::Float64 = -Inf
    for 可能目标 in 敌方队伍(战场, 来源)
        local 伤害 = 期望函数(可能目标)
        if 伤害 > 当前最优的伤害
            当前最优的目标 = 可能目标
            当前最优的伤害 = 伤害
        end
    end
    return 当前最优的目标 => 当前最优的伤害
end

# 选择单体攻击的目标，返回目标和预期伤害
function 选择单体攻击的目标(战场::战场, 来源::角色, 威力::Int)
    期望函数 = (目标::角色) -> 攻击预期伤害(来源, 目标, 威力)
    return 根据期望函数选择目标(战场, 来源, 期望函数)
end

# 必须在技能定义中调用这个宏
macro 单体攻击(说明::Expr)

    威力 = 提取字段(:威力, 说明)
    削韧 = 提取字段(:削韧, 说明, 默认值 = 0)
    解析器.对目标的效果 = :(
        @攻击并削韧 begin
            威力 = $威力
            削韧 = $削韧
            目标 = $(esc(:目标))
        end
    )

    push!(解析器.默认性质列表, 单体攻击)
    解析器.已解析到生效 = true

end

push!(技能定义关键字, Symbol("@单体攻击"))