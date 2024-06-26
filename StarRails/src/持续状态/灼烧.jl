# 灼烧效果

@创建持续状态 灼烧 负面状态 begin

    威力::Int

    function 灼烧(事件::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 威力::Int, 持续时间::Int = 1, 性质::Vector = [])
        new(事件.战场, 来源, 目标, 持续时间, [事件.性质列表..., 性质...], 威力)
    end

end

function 回合开始时触发(事件::回合开始, 状态::灼烧)
    攻击目标!(事件, 
        威力 = 状态.威力,
        来源 = 状态.来源,
        目标 = 状态.目标,
        性质 = [持续伤害, 灼烧伤害]
    )
    状态.持续时间 -= 1
end

是持续伤害类状态(状态::灼烧) = true