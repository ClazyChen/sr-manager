# 根据标签增加某种攻击的威力效果

mutable struct 攻击强效 <: 持续状态
    战场::战场
    来源::角色
    目标::角色
    持续时间::Int
    性质列表::Vector{标签.性质}
    强效标签::标签.性质
    数值::Int

    function 攻击强效(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 持续时间::Int = 1, 数值::Int, 性质::Vector = [], 强效标签::标签.性质)
        new(事件.战场, 来源, 目标, 持续时间, [事件.性质列表..., 性质...], 强效标签, 数值)
    end
end

状态名称(状态::攻击强效) = "$(状态.强效标签)▲"
状态定性(状态::攻击强效) = 正面状态

function 回合结束时触发(事件::回合结束, 状态::攻击强效)
    状态.持续时间 -= 1
end

