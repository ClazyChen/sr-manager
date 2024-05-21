# 属性提升类效果的模板

macro 属性提升类效果模板(提升效果::Symbol)
    提升效果名 = Symbol("$(提升效果)▲")
    quote
        @创建持续状态 $提升效果名 正面状态 begin
            
            $(esc(:数值))::Int

            function $(esc(提升效果名))($(esc(:事件))::事件; 来源::角色 = 事件.来源, 目标::角色 = 事件.目标, 持续时间::Int = 1, 数值::Int)
                $(esc(:new))(事件.战场, 来源, 目标, 持续时间, 数值)
            end
        end

        function $(esc(:附加持续状态时触发))(事件::附加持续状态, 状态::$提升效果名)
            事件.目标.$提升效果 += 状态.数值
        end

        function $(esc(:回合结束时触发))(事件::回合结束, 状态::$提升效果名)
            状态.持续时间 -= 1
        end

        function $(esc(:脱离持续状态时触发))(事件::脱离持续状态, 状态::$提升效果名)
            事件.目标.$提升效果 -= 状态.数值
        end

    end
end

@属性提升类效果模板 攻击
@属性提升类效果模板 防御
@属性提升类效果模板 穿甲
@属性提升类效果模板 暴击
@属性提升类效果模板 效果命中
@属性提升类效果模板 效果抵抗