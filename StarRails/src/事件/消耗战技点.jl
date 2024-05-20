# 消耗战技点的效果

@创建事件 消耗战技点 begin

    数值::Int

    function 消耗战技点(战场::战场, 目标::角色; 数值::Int = 1, 性质::Array{标签.性质}=[])
        new(战场, 性质, 目标, 目标, 数值)
    end

end

@触发事件 begin
    战技点变化!(事件.战场, 角色 = 事件.目标, 数值 = - 事件.数值)
end

macro 消耗战技点(数值::Int)
    quote
        $(esc(:触发))(消耗战技点(
            $(esc(:事件)).战场,
            $(esc(:事件)).目标,
            数值 = $(esc(数值)),
            性质 = $(esc(:事件)).性质列表))
    end
end