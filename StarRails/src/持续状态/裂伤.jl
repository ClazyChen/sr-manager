# 裂伤效果

@创建持续状态 裂伤 begin

    威力::Int

    function 裂伤(战场::战场; 来源::角色, 目标::角色, 威力::Int, 持续时间::Int = 1)
        new(战场, 负面状态, 来源, 目标, 持续时间, 威力)
    end

    function 裂伤(事件::事件; 威力::Int, 持续时间::Int = 1)
        new(事件.战场, 负面状态, 事件.来源, 事件.目标, 持续时间, 威力)
    end

end

function 回合开始时触发(事件::回合开始, 状态::裂伤)
    攻击目标!(事件, 
        威力 = 状态.威力,
        来源 = 状态.来源,
        目标 = 状态.目标,
        性质 = [裂伤伤害]
    )
    状态.持续时间 -= 1
end