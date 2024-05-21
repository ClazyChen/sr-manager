# 护盾效果，会影响攻击时的结算流程

@创建持续状态 护盾 正面状态 begin
    
    数值::Int

    function 护盾(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 数值::Int, 持续时间::Int = 1)
        new(事件.战场, 来源, 目标, 持续时间, 数值)
    end

end

function 回合开始时触发(事件::回合开始, 状态::护盾)
    状态.持续时间 -= 1
end

function 盾值(角色::角色)
    最大盾值 = 0
    for 状态 in 角色.状态列表
        if 状态 isa 护盾
            最大盾值 = max(状态.数值, 最大盾值)
        end
    end
    最大盾值
end