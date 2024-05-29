# 包含所有角色的数据库

include("星穹列车/开拓者物理.jl")
include("星穹列车/丹恒.jl")
include("星穹列车/姬子.jl")
include("星穹列车/瓦尔特.jl")
include("星穹列车/三月七.jl")
include("黑塔空间站/黑塔.jl")
include("黑塔空间站/阿兰.jl")
include("黑塔空间站/艾丝妲.jl")
include("贝洛伯格/开拓者火.jl")
include("贝洛伯格/桑博.jl")
include("贝洛伯格/布洛妮娅.jl")
include("贝洛伯格/娜塔莎.jl")
include("贝洛伯格/虎克.jl")
include("贝洛伯格/克拉拉.jl")

using InteractiveUtils

# 对 subtypes(角色模板) 中的每个类型，创建一个实例
角色数据库 = [T() for T in subtypes(角色模板)]