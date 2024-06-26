# 致命（即将陷入无法战斗）的结算
# 用于触发白露、杰帕德等复活效果

@创建事件 致命 begin
    function 致命(事件::事件)
        new(事件.战场, 事件.性质列表, 事件.来源, 事件.目标)
    end
end

@触发事件 begin
    if 事件.目标.生命.当前 <= 0
        消灭目标!(事件)
    end
end

function 造成致命伤害!(事件::事件)
    触发(致命(事件))
end
