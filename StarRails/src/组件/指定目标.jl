# 指定目标

include("单体攻击.jl")
include("扩散攻击.jl")
include("全体攻击.jl")
include("随机攻击.jl")

# 选择目标的函数
function _指定目标!(战场::战场, 技能::技能)::Float64
    if 单体攻击 in 技能
        目标选择结果 = 选择单体攻击的目标(战场, 技能.来源, 技能.威力)
        技能.目标 = [目标选择结果[1]]
        return 目标选择结果[2]
    end
    if 扩散攻击 in 技能
        目标选择结果 = 选择扩散攻击的目标(战场, 技能.来源, 技能.主目标威力, 技能.副目标威力)
        技能.目标 = [目标选择结果[1]]
        左侧副目标 = 左侧角色(战场, 目标选择结果[1])
        右侧副目标 = 右侧角色(战场, 目标选择结果[1])
        if 左侧副目标 !== nothing
            push!(技能.目标, 左侧副目标)
        end
        if 右侧副目标 !== nothing
            push!(技能.目标, 右侧副目标)
        end
        return 目标选择结果[2]
    end
    if 全体攻击 in 技能
        目标选择结果 = 选择全体攻击的目标(战场, 技能.来源, 技能.威力)
        技能.目标 = 目标选择结果[1]
        return 目标选择结果[2]
    end
    if 随机攻击 in 技能
        目标选择结果 = 选择随机攻击的目标(战场, 技能.来源, 技能.威力, 技能.次数)
        技能.目标 = 随机选择(目标选择结果[1], 技能.次数)
        return 目标选择结果[2]
    end
    if 全体辅助 in 技能
        技能.目标 = 我方队伍(战场, 技能.来源)
        return 100
    end
    if 单体辅助 in 技能
        目标选择结果 = 根据期望函数选择队友(战场, 技能.来源, 角色 -> 
            角色 == 技能.来源 ? -Inf :
            10 * (角色.攻击 + 角色.暴击 + 角色.穿甲)
        )
        if 目标选择结果[1] === nothing
            return -Inf
        end
        技能.目标 = [目标选择结果[1]]
        return 战技点(战场, 技能.来源).当前 <= 1 ? -Inf : 目标选择结果[2]
    end
    if 单体护盾 in 技能
        当前选中的目标的生命 = Inf
        for 目标 in 我方队伍(战场, 技能.来源)
            if 目标.生命.当前 < 当前选中的目标的生命 && 目标.生命.当前 < 目标.生命.上限 / 2 && 盾值(目标) == 0
                当前选中的目标的生命 = 目标.生命.当前
                技能.目标 = [目标]
                return 技能.护盾值 * 10
            end
        end
        当前选中的目标的防御 = -Inf
        for 目标 in 我方队伍(战场, 技能.来源)
            if 目标.防御 > 当前选中的目标的防御 && 盾值(目标) == 0
                当前选中的目标的防御 = 目标.防御
                技能.目标 = [目标]
                return 技能.护盾值 * 10
            end
        end
        技能.目标 = [技能.来源]
        return 技能.护盾值 * 10 - 盾值(技能.来源)
    end
    return -Inf
end

function 指定目标!(战场::战场, 技能::技能)::Float64
    基础收益 = _指定目标!(战场, 技能)
    当前存活的角色数 = length(我方队伍(战场, 技能.来源))
    战技点缺口 = max(当前存活的角色数 - 1 - 战技点(战场, 技能.来源).当前, 0)
    战技点收益 = 普攻 in 技能 ? 战技点缺口 * (战技点(战场, 技能.来源).上限 - 战技点(战场, 技能.来源).当前) : 0 
    return 基础收益 + 战技点收益
end

# 指定目标方式的宏
macro 目标(指定方式)
    if 指定方式 isa Symbol
        push!(解析器.默认性质列表, eval(指定方式))
    else
        解析器.是否显式指定目标 = true
        解析器.显式指定目标方法 = 指定方式
    end
end
