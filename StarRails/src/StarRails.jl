module StarRails

include("随机.jl")
include("颜色.jl")
include("标签.jl")
include("常数.jl")

include("控制台/界面控制.jl")
include("组件/战场.jl")

# 加载控制台界面
include("控制台/选择角色界面.jl")
include("控制台/战场交互界面.jl")

# 加载事件
include("事件/事件.jl")

# 加载技能
include("组件/技能.jl")

# 加载角色
include("角色/角色数据库.jl")

include("组件/游戏流程.jl")

# 启动游戏
function 启动()
    战场 = 运行()
    执行游戏流程!(战场)
end

end