# 创建角色的上下文，用于通过宏来创建角色

mutable struct 上下文
    角色名::Symbol
    事件名::Symbol
    技能名::Symbol
    当前角色的持有技能::Array{Symbol}
    上下文() = new(:无, :无, :无)
end

# 全局实现一个角色上下文
const 当前上下文 = 上下文()
