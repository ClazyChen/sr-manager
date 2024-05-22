# 创建角色的上下文，用于通过宏来创建角色

mutable struct 上下文
    角色名::Symbol
    事件名::Symbol
    技能名::Symbol
    当前角色的持有技能::Vector{Symbol}
    是否不显示下一条信息::Bool
    上下文() = new(:无, :无, :无, [], false)
end

# 全局实现一个角色上下文
const 当前上下文 = 上下文()

macro 不显示()
    quote
        当前上下文.是否不显示下一条信息 = true
    end
end
