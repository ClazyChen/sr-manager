# 根据标签抗性降低类效果

mutable struct 抗性降低 <: 持续状态
    战场::战场
    来源::角色
    目标::角色
    持续时间::Int
    性质列表::Vector{标签.性质}
    抗性标签::标签.性质
    数值::Int

    function 抗性降低(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 持续时间::Int = 1, 数值::Int, 性质::Vector = [], 抗性标签::标签.性质)
        new(事件.战场, 来源, 目标, 持续时间, [事件.性质列表..., 性质...], 抗性标签, 数值)
    end
end

状态名称(状态::抗性降低) = "$(状态.抗性标签)▼"
状态定性(状态::抗性降低) = 负面状态

function 回合结束时触发(事件::回合结束, 状态::抗性降低)
    状态.持续时间 -= 1
end

