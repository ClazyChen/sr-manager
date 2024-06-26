@创建角色 艾丝妲 begin
    属性 = 火
    命途 = 同谐
    生命 = 160
    能量 = 进度"0/3"
    技力 = 进度"0/5"
    攻击 = 9
    防御 = 7
    速度 = 11
    命中 = 0
    抵抗 = 0
    暴击 = 1
    穿甲 = 2
end

@创建技能 普攻 光谱射线 begin
    @标准普攻模板
    @效果 使用者(技能) begin
        目标.技力 += 1
    end
end

@创建技能 战技 流星群落 begin
    @定义 威力 75
    @定义 次数 5
    @战技模板 begin
        @随机攻击
        @效果 使用者(技能) begin
            目标.技力 += length(不重复目标(技能))
        end
    end
end

@创建技能 终结技 星空祝言 begin
    @定义 速度提高 4
    @终结技模板 begin
        @目标 全体辅助
        @效果 所有目标(技能) begin
            附加持续状态!(事件, 状态 = 加速(事件, 数值 = 技能.速度提高, 持续时间 = 2))
        end
    end
end

@创建技能 被动 天象学 begin
    @定义 回合开始时消耗技力 3
    @定义 回合开始前当前技力 0
    @定义 回合开始时当前技力 0
    @时机 回合开始时
    @时机 回合结束时
    @条件 begin
        事件.来源 == 角色 && @不显示
    end
    @效果 [事件.目标] begin
        if 事件 isa 回合开始
            技能.来源.技力.当前 -= 技能.回合开始时消耗技力
            if 技能.来源.技力.当前 < 0
                技能.来源.技力.当前 = 0
            end
            技能.回合开始时当前技力 = 技能.来源.技力.当前
            for 目标 in 使用者全队(技能, 事件)
                目标.攻击 -= 技能.回合开始前当前技力 - 技能.回合开始时当前技力
            end
            技能.来源.防御 -= 技能.回合开始前当前技力 - 技能.回合开始时当前技力
        else
            技能.回合开始前当前技力 = 技能.来源.技力.当前
            for 目标 in 使用者全队(技能, 事件)
                目标.攻击 += 技能.回合开始前当前技力 - 技能.回合开始时当前技力
            end
            技能.来源.防御 += 技能.回合开始前当前技力 - 技能.回合开始时当前技力
        end
    end
end

@创建技能 被动 灵光一现 begin
    @入场技能模板 begin
        @效果 使用者敌队(技能, 事件) begin
            削韧目标!(事件, 目标 = 目标, 数值 = 4)
        end
    end
end

@角色创建完成