# 复苏效果，回合开始时回复生命

@创建持续状态 复苏 正面状态 begin
    
    数值::Int

    function 复苏(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.来源, 数值::Float64, 持续时间::Int = 1, 性质::Vector = [])
        new(事件.战场, 来源, 目标, 持续时间, [事件.性质列表..., 性质...], 数值 * 10)
    end

end

function 回合开始时触发(事件::回合开始, 状态::复苏)
    治疗目标!(事件, 数值 = 状态.数值, 目标 = 状态.目标, 来源 = 状态.来源, 性质 = 状态.性质列表)
    状态.持续时间 -= 1
end