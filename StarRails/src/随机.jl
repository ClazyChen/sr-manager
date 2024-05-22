# 定义了产生随机数的函数

module 随机

随机数(范围::UnitRange{Int})::Int = rand(范围)
随机判定(阈值::Int)::Bool = 随机数(1:6) <= 阈值
随机选择(列表::Vector) = 列表[随机数(1:length(列表))]
随机选择(列表::Vector, 数量::Int) = [随机选择(列表) for _ in 1:数量]

export 随机数, 随机判定, 随机选择

end

using .随机