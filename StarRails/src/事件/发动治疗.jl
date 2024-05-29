@创建事件 发动治疗 begin
    
    数值::Int
    造成治疗量::Int
    溢出治疗量::Int

    function 发动治疗(战场::战场; 来源::角色, 目标::角色, 数值::Int, 性质::Vector{标签.性质}=[])
        new(战场, 性质, 来源, 目标, 数值, 数值, 0)
    end

end

@触发事件 begin
    
    触发时机!(事件, 治疗开始时)    

    if 事件.造成治疗量 + 事件.目标.生命.当前 > 事件.目标.生命.上限
        事件.溢出治疗量 = 事件.造成治疗量 + 事件.目标.生命.当前 - 事件.目标.生命.上限
        事件.造成治疗量 = 事件.目标.生命.上限 - 事件.目标.生命.当前
    end

    if 事件.造成治疗量 > 0
        控制台换行("$事件")
        回复生命!(事件, 事件.造成治疗量)    
    end

end

function Base.show(io::IO, 事件::发动治疗)
    print(io, "【$(事件.来源)】对【$(事件.目标)】进行治疗，恢复了 $(事件.造成治疗量 / 10.0) 点生命。")
end

function 治疗目标!(事件::事件; 数值, 来源 = 事件.来源, 目标 = 事件.目标, 性质::Vector{标签.性质} = 事件.性质列表)
    if 数值 isa AbstractFloat
        数值 = round(Int, 数值 * 10)
    end
    触发(发动治疗(事件.战场, 来源 = 来源, 目标 = 目标, 数值 = 数值, 性质 = 性质))
end
