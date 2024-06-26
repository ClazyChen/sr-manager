# 所有事件的基类
abstract type 事件 end

# 事件所具有的性质
性质列表(事件::事件) = 事件.性质列表
Base.in(性质::标签.性质, 事件::事件)::Bool = 性质 in 性质列表(事件)

# 调用事件
触发(事件::事件) = nothing

# 从事件说明中提取对应的字段
function 提取字段(字段名::Symbol, 说明::Expr; 默认值 = nothing)
    for i in 2:2:length(说明.args)
        字段 = 说明.args[i].args[1]
        值 = 说明.args[i].args[2]
        if 字段 == 字段名
            return 值
        end
    end
    return 默认值
end

# 每个事件的基本要素
macro 创建事件(事件名::Symbol, 事件定义::Expr)
    当前上下文.事件名 = 事件名
    quote
        mutable struct $事件名 <: 事件
            战场::战场
            性质列表::Vector{标签.性质}
            来源::角色
            目标::角色
            $(esc(事件定义))
        end
    end
end

# 触发事件
macro 触发事件(效果::Expr)
    quote
        function $(esc(:触发))($(esc(:事件))::$(当前上下文.事件名))
            if 事件.目标.存活
                $(esc(效果))
            end
        end
    end
end

function _触发时机!(事件::事件, 角色::角色, 时机::标签.触发时机)
    已经结束(事件.战场) && return
    for 技能 in 角色.技能列表
        if 被动 in 技能 && 时机 in 技能.触发时机 && 可以使用(技能, 事件)
            控制台换行("【$角色】触发了被动技能【$技能】。")
            触发(技能, 事件)
            已经结束(事件.战场) && return
        end
    end
end

function 触发时机!(事件::事件, 时机::标签.触发时机)
    按照结算次序进行(事件.战场, 操作 = 角色 -> _触发时机!(事件, 角色, 时机))
end

# 空事件
@创建事件 空事件 begin 
    function 空事件(战场::战场, 目标::角色)
        new(战场, [], 目标, 目标)
    end
end

# 将所有事件包含进来
include("进入战斗.jl")
include("../持续状态/持续状态.jl")
include("解除负面状态.jl")
include("消灭.jl")
include("致命.jl")
include("损失生命.jl")

include("../持续状态/护盾.jl")
include("../持续状态/抗性降低.jl")
include("../持续状态/攻击强效.jl")
include("发动攻击.jl")

include("回复生命.jl")
include("发动治疗.jl")
include("../持续状态/复苏.jl")

include("../持续状态/裂伤.jl")
include("../持续状态/灼烧.jl")
include("../持续状态/冻结.jl")
include("../持续状态/触电.jl")
include("../持续状态/风化.jl")
include("../持续状态/减速.jl")
include("../持续状态/加速.jl")
include("../持续状态/属性提升.jl")
include("../持续状态/属性降低.jl")
include("../持续状态/嘲讽.jl")

include("造成击破.jl")
include("造成削韧.jl")

include("获得战技点.jl")
include("消耗战技点.jl")
include("获得能量.jl")
include("消耗能量.jl")
