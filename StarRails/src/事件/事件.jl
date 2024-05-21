# 所有事件的基类
abstract type 事件 end

# 事件所具有的性质
性质列表(事件::事件)::Array{标签.性质} = 事件.性质列表
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
            性质列表::Array{标签.性质}
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
    for 技能 in 角色.技能列表
        if 被动 in 技能 && 时机 in 技能.触发时机 && 可以使用(事件.战场, 技能, 事件)
            控制台换行("【$角色】触发了被动技能【$技能】。")
            触发(事件.战场, 技能, 事件)
        end
    end
end

function 触发时机!(事件::事件, 时机::标签.触发时机)
    蓝方角色 = 事件.战场.蓝方角色
    红方角色 = 事件.战场.红方角色
    for i in 1:max(length(蓝方角色), length(红方角色))
        if i <= length(蓝方角色)
            _触发时机!(事件, 蓝方角色[i], 时机)
        end
        已经结束(事件.战场) && break
        if i <= length(红方角色)
            _触发时机!(事件, 红方角色[i], 时机)
        end
        已经结束(事件.战场) && break
    end
end

# 将所有事件包含进来
include("消灭.jl")
include("致命.jl")
include("损失生命.jl")
include("发动攻击.jl")
include("造成击破.jl")
include("造成削韧.jl")
include("获得战技点.jl")
include("消耗战技点.jl")
include("获得能量.jl")
include("消耗能量.jl")

# 空事件
struct 空事件 <: 事件 end

const 事件占位符 = 空事件()