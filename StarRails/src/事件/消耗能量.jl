# 消耗能量的效果

@创建事件 消耗能量 begin

    数值::Int

    function 消耗能量(战场::战场, 指向; 数值::Int = 1, 性质::Array{标签.性质}=[])
        new(战场, 性质, 指向.first, 指向.second, 数值)
    end

end

@触发事件 begin
    事件.目标.能量 -= 事件.数值
end

macro 消耗能量(数值::Int)
    quote
        $(esc(:触发))(消耗能量(
            $(esc(:事件)).战场,
            $(esc(:事件)).来源 => $(esc(:事件)).目标,
            数值 = $(esc(数值)),
            性质 = $(esc(:事件)).性质列表))
    end
end