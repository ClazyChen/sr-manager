# 技能的基类
# 仅用于主动技能，被动技能使用触发器实现
abstract type 技能 end

Base.show(io::IO, 技能::技能) = print(io, 技能.名称)

# 技能所具有的性质
性质列表(技能::技能)::Array{标签.性质} = 技能.性质列表
Base.in(性质::标签.性质, 技能::技能)::Bool = 性质 in 性质列表(技能)

# 使用技能
触发(战场::战场, 技能::技能, 事件::事件) = error("未定义技能$(技能)")

# 是否可以使用技能
可以使用(战场::战场, 技能::技能)::Bool = true

include("技能解析器.jl")
include("变量定义.jl")
include("单体攻击.jl")
include("../事件/使用技能.jl")

# 每个技能的基本要素
macro 创建技能(技能类型::Symbol, 技能名::Symbol, 技能定义::Expr)

    当前上下文.技能名 = 技能名
    push!(当前上下文.当前角色的持有技能, 技能名)
    重置解析器(eval(技能类型))    
    
    for 语句 in 技能定义.args
        if 语句 isa Expr
            if 语句.args[1] in 技能定义关键字
                eval(语句)
            else
                if !解析器.已解析到生效
                    push!(解析器.生效前流程, 语句)
                else
                    push!(解析器.生效后流程, 语句)
                end
            end
        end
    end

    私有变量定义::Array{Expr} = []
    私有变量默认值::Array{Any} = []
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
            性质列表::Array{标签.性质}
            来源::角色
            目标::Array{角色}

            $(私有变量定义...)

            function $技能名($(esc(:使用者))::角色)
                new($技能名字符串, [$(解析器.默认性质列表...)], 使用者, [], $(私有变量默认值...))
            end
        end

        function $(esc(:触发))($(esc(:战场))::战场, $(esc(:技能))::$(esc(技能名)), $(esc(:事件))::事件)
            $(解析器.生效前流程...)
            for $(esc(:目标)) in 技能.目标
                $(解析器.对目标的效果)
            end
            $(解析器.生效后流程...)
        end

    end
end
