# 包含一个当前进度和进度上限

mutable struct 进度
    当前::Int
    上限::Int
    放大十倍::Bool

    function 进度(; 当前::Int, 上限::Int, 放大十倍::Bool = false)
        当前 = min(当前, 上限)
        new(当前, 上限, 放大十倍)
    end
end

# 进度强制转换为Int（只保留当前）
Base.convert(::Type{Int}, x::进度) = x.当前

# 重载 + - += -= 运算符，只对当前进行操作
Base.:+(x::进度, y::Int) = 进度(当前 = x.当前 + y, 上限 = x.上限, 放大十倍 = x.放大十倍)
Base.:-(x::进度, y::Int) = x + (-y)
Base.:+(x::Int, y::进度) = y + x
Base.:-(x::Int, y::进度) = y + (-x)

Base.:+(x::进度, y::AbstractFloat) = 进度(当前 = x.当前 + round(Int, y * 10), 上限 = x.上限, 放大十倍 = x.放大十倍)
Base.:-(x::进度, y::AbstractFloat) = x + (-y)
Base.:+(x::AbstractFloat, y::进度) = y + x
Base.:-(x::AbstractFloat, y::进度) = y + (-x)

Base.:>>(x::进度, y::Int) = 进度(当前 = x.当前 + y, 上限 = x.上限 + y, 放大十倍 = x.放大十倍)
Base.:>>(x::进度, y::AbstractFloat) = 进度(当前 = x.当前 + round(Int, y * 10), 上限 = x.上限 + round(Int, y * 10), 放大十倍 = x.放大十倍)

# 可以使用 进度"1/3" 来创建一个进度
macro 进度_str(x)
    split_str = split(x, '/')
    return esc(:(进度(当前 = parse(Int, $(split_str[1])), 上限 = parse(Int, $(split_str[2])))))
end

# 用于显示进度
function Base.show(io::IO, x::进度) 
    if x.放大十倍
        # 除以 10 保留 1 位小数输出
        print(io, "$(x.当前 / 10.0) / $(x.上限 / 10.0)")
    else
        print(io, "$(x.当前) / $(x.上限)")
    end
end