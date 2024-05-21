# 技能的基类
# 仅用于主动技能，被动技能使用触发器实现
abstract type 技能 end

Base.show(io::IO, 技能::技能) = print(io, 技能.名称)

# 技能所具有的性质
性质列表(技能::技能)::Vector{标签.性质} = 技能.性质列表
Base.in(性质::标签.性质, 技能::技能)::Bool = 性质 in 性质列表(技能)

# 使用技能
触发(技能::技能, 事件::事件) = error("未定义技能$(技能)")

# 是否可以使用技能
可以使用(技能::技能, 事件::事件)::Bool = true

include("技能解析器.jl")
include("变量定义.jl")
include("发动条件.jl")
include("触发时机.jl")
include("产生效果.jl")
include("指定目标.jl")
include("使用技能.jl")

# 每个技能的基本要素
macro 创建技能(技能类型::Symbol, 技能名::Symbol, 技能定义::Expr)

    当前上下文.技能名 = 技能名
    push!(当前上下文.当前角色的持有技能, 技能名)
    重置解析器(eval(技能类型))    
    
    eval(技能定义)

    私有变量定义::Vector{Expr} = []
    私有变量默认值::Vector{Any} = []
    for 私有变量 in 解析器.私有变量列表
        私有变量名 = 私有变量[1]
        私有变量值 = 私有变量[2]
        私有变量类型 = typeof(私有变量值)
        push!(私有变量定义, esc(:($私有变量名::$私有变量类型)))
        push!(私有变量默认值, 私有变量值)
    end

    技能名字符串 = string(技能名)

    quote

        mutable struct $技能名 <: 技能

            名称::String
            性质列表::Vector{标签.性质}
            触发时机::Vector{标签.触发时机}
            来源::角色
            目标::Vector{角色}
            指定目标::Function

            $(私有变量定义...)

            function $技能名($(esc(:使用者))::角色)
                new($技能名字符串, [$(解析器.默认性质列表...)], [$(解析器.触发时机...)], 使用者, [], 指定目标!, $(私有变量默认值...))
            end
        end

        function $(esc(:触发))(技能::$(esc(技能名)), $(esc(:事件))::事件)
            来源 = 技能.来源
            $(解析器.效果列表...)
        end

        function $(esc(:可以使用))(技能::$(esc(技能名)), $(esc(:事件))::事件)::Bool
            角色 = 技能.来源
            $(解析器.判定条件)
        end

    end
end
