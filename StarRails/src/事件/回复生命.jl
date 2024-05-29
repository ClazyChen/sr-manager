@创建事件 回复生命 begin
    
    数值::Int
    回复前的生命值::Int
    回复后的生命值::Int

    function 回复生命(战场::战场; 来源::角色, 目标::角色, 数值::Int, 性质::Vector{标签.性质}=[])
        new(战场, 性质, 来源, 目标, 数值, 目标.生命.当前, min(目标.生命.当前 + 数值, 目标.生命.上限))
    end

end

@触发事件 begin
    事件.目标.生命 += 事件.数值
    控制台换行("$事件")
end

Base.show(io::IO, 事件::回复生命) = print(io, "【$(事件.目标)】生命值回复 $(事件.数值 / 10.0) 点，恢复至 $(事件.目标.生命.当前 / 10.0) 点。")

function 回复生命!(事件::事件, 数值::Int)
    触发(回复生命(事件.战场, 来源 = 事件.来源, 目标 = 事件.目标, 数值 = 数值, 性质 = 事件.性质列表))
end

回复生命!(事件::事件; 数值::AbstractFloat) = 回复生命!(事件, round(Int, 数值 * 10))