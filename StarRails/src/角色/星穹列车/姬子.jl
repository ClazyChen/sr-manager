@创建角色 姬子 begin
    属性 = 火
    命途 = 智识
    生命 = 160
    能量 = 进度"0/3"
    技力 = 进度"0/0"
    攻击 = 14
    防御 = 4
    速度 = 9
    命中 = 0
    抵抗 = 1
    暴击 = 0
    穿甲 = 2
end

@创建技能 普攻 武装调律 begin
    @标准普攻模板
end

@创建技能 战技 熔核爆裂 begin
    @定义 主目标威力 200
    @定义 副目标威力 100
    @战技模板 begin
        @扩散攻击
    end
end

@创建技能 终结技 天坠之火 begin
    @定义 威力 230
    @终结技模板 begin
        @全体攻击
    end
end

@创建技能 被动 乘胜追击 begin
    @时机 攻击结束时
    @条件 begin
        事件.来源 == 角色 && 事件.造成伤害 > 0 && 事件.目标.存活
    end
    @效果 [事件.目标] begin
        附加持续状态!(事件, 状态 = 灼烧(事件, 威力 = 60, 持续时间 = 2), 命中 = 3)
    end
end

@创建技能 被动 不完全燃烧 begin
    @入场技能模板 begin
        @效果 使用者敌队(技能, 事件) begin
            附加持续状态!(事件, 目标 = 目标, 状态 = 灼烧(事件, 目标 = 目标, 威力 = 60, 持续时间 = 2), 命中 = 6)
        end
    end
end

@角色创建完成