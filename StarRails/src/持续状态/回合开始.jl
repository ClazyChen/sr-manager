# 回合开始的过程
# 用于触发回合开始时的技能
# 以及刷新持续状态

@创建事件 回合开始 begin

    function 回合开始(战场::战场, 目标::角色)
        new(战场, [], 目标, 目标)
    end

end

回合开始时触发(事件::回合开始, 状态::持续状态) = nothing

# 和卡芙卡等触发持续伤害的技能有所的区别
function _触发所有状态(事件::事件, 触发方式::Function)
    本回合已经触发的状态 = []
    while true
        仍然有未触发的状态 = false
        当前状态列表长度 = length(事件.目标.状态列表)
        for i in 1:当前状态列表长度
            i > length(事件.目标.状态列表) && break
            状态 = 事件.目标.状态列表[i]
            状态 in 本回合已经触发的状态 && continue
            触发方式(事件, 状态)
            已经结束(事件.战场) && return
            push!(本回合已经触发的状态, 状态)
            仍然有未触发的状态 = true
        end
        仍然有未触发的状态 || break
    end
    for 状态 in 事件.目标.状态列表
        if 状态.持续时间 == 0
            脱离持续状态!(事件, 状态)
            已经结束(事件.战场) && return
        end
    end
    filter!(状态 -> 状态.持续时间 > 0, 事件.目标.状态列表)
end

@触发事件 begin
    _触发所有状态(事件, 回合开始时触发)
    触发时机!(事件, 回合开始时)
end

function 回合开始!(战场::战场)
    触发(回合开始(战场, 当前角色(战场.速度条)))
end