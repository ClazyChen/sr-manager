# 包含所有角色的数据库

include("星穹列车/开拓者物理.jl")
include("星穹列车/丹恒.jl")
include("星穹列车/姬子.jl")
include("星穹列车/瓦尔特.jl")
include("星穹列车/三月七.jl")

using InteractiveUtils

# 对 subtypes(角色模板) 中的每个类型，创建一个实例
角色数据库 = [T() for T in subtypes(角色模板)]