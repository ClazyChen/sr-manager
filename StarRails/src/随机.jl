# 定义了产生随机数的函数

module 随机

随机数(范围::UnitRange{Int})::Int = rand(范围)
随机判定(阈值::Int)::Bool = 随机数(1:6) <= 阈值

export 随机数, 随机判定

end