# 脱离持续状态
# 注意此处为自然脱离，即在状态的持续时间结束时自动解除，而非强制解除

@创建事件 脱离持续状态 begin
    
    状态::持续状态

    function 脱离持续状态(战场::战场; 状态::持续状态, 性质::Vector = [])
        new(战场, 性质, 状态.来源, 状态.目标, 状态)
    end

end

脱离持续状态时触发(事件::脱离持续状态, 状态::持续状态) = nothing

@触发事件 begin
    脱离持续状态时触发(事件, 事件.状态)
end

function 脱离持续状态!(事件::事件; 状态::持续状态)
    触发(脱离持续状态(事件.战场, 状态 = 状态, 性质 = 事件.性质列表))
end