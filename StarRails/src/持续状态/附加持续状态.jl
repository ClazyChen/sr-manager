# 附加持续状态
@创建事件 附加持续状态 begin
    
    状态::持续状态

    基础命中::Int
    来源效果命中::Int
    目标效果抵抗::Int
    是否命中::Bool

    function 附加持续状态(战场::战场; 来源::角色, 目标::角色, 状态::持续状态, 性质::Vector = [], 命中::Int = 100)
        new(战场, 性质, 来源, 目标, 状态, 命中, 来源.效果命中, 目标.效果抵抗, false)
    end

end

附加持续状态时触发(事件::附加持续状态, 状态::持续状态) = nothing

@触发事件 begin
    实际命中 = 事件.基础命中 + 事件.来源效果命中 - 事件.目标效果抵抗
    事件.是否命中 = 随机判定(实际命中)
    if 事件.是否命中
        push!(事件.目标.状态列表, 事件.状态)
        附加持续状态时触发(事件, 事件.状态)
    end
    控制台换行("$事件")
end

function Base.show(io::IO, 事件::附加持续状态)
    命中说明 = ""
    if 事件.是否命中
        命中说明 = @绿 "成功"
    else
        命中说明 = @红 "抵抗"
    end
    print(io, "【$(事件.来源)】对【$(事件.目标)】附加了【$(事件.状态)】（$命中说明）。")
end

function 附加持续状态!(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 状态::持续状态, 性质::Vector{标签.性质} = 事件.性质列表, 命中::Int = 100)::Bool
    附加持续状态的事件 = 附加持续状态(事件.战场, 来源 = 来源, 目标 = 目标, 状态 = 状态, 性质 = 性质, 命中 = 命中)
    触发(附加持续状态的事件)
    附加持续状态的事件.是否命中
end