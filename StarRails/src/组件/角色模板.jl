# 角色模板，每个角色是唯一的，用于判定是否是某一个角色
# 在进入战斗时会使用模板生成一个角色实例，用于记录角色的状态
abstract type 角色模板 end

# 在控制台上输出角色的信息
Base.show(io::IO, 模板::角色模板) = print(io, "$(模板.名称)[模板]")

include("上下文.jl")

# 继承角色模板的类型，需要实现一个无参构造函数，用于生成角色模板实例
# 在该函数中需要定义所有的角色基本属性

macro 创建角色(角色名::Symbol, 属性列表::Expr)
    local 模板前缀 = "_模板_$角色名"
    local 角色名字符串 = string(角色名)
    local 新属性列表::Vector{Symbol} = []
    当前上下文.角色名 = 角色名
    当前上下文.当前角色的持有技能 = []

    # 给“属性列表”中的每个“属性 = 值”的语句改为 
    # const 模板前缀_属性 = 值
    for i in 2:2:length(属性列表.args)
        新属性名 = Symbol(模板前缀 * "_$(属性列表.args[i].args[1])")
        push!(新属性列表, 新属性名)
        属性列表.args[i] = esc(:(const $新属性名 = $(属性列表.args[i].args[2])))
    end

    quote
        $属性列表

        struct $角色名 <: 角色模板
            名称::String
            属性::标签.属性
            命途::标签.命途
            生命::Int
            能量::进度
            攻击::Int
            防御::Int
            速度::Int
            命中::Int
            抵抗::Int
            暴击::Int
            穿甲::Int

            function $角色名()
                new($角色名字符串, $(新属性列表...))
            end
        end
    end
end

macro 角色创建完成()

    生成技能::Vector{Expr} = []
    for 技能名 in 当前上下文.当前角色的持有技能
        push!(生成技能, quote
            push!(角色.技能列表, $(技能名)(角色))
        end)
    end

    quote
        function $(esc(:初始化技能!))($(esc(:角色))::角色, $(esc(:模板))::$(当前上下文.角色名))
            $(生成技能...)
        end
    end

end