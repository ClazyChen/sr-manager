@创建角色 布洛妮娅 begin
    属性 = 风
    命途 = 同谐
    生命 = 155
    能量 = 进度"0/3"
    技力 = 进度"0/0"
    攻击 = 11
    防御 = 5
    速度 = 10
    命中 = 0
    抵抗 = 1
    暴击 = 2
    穿甲 = 2
end

@创建技能 普攻 驭风的子弹 begin
    @标准普攻模板
end

@创建技能 战技 作战再部署 begin
    @定义 穿甲提高 4
    @战技模板 begin
        @目标 单体辅助
        @效果 所有目标(技能) begin
            拉条!(事件.战场.速度条, 目标, 10)
            解除负面状态!(事件)
            附加持续状态!(事件, 状态 = 穿甲▲(事件, 数值 = 技能.穿甲提高))
        end
    end
end

@创建技能 终结技 贝洛伯格进行曲 begin
    @定义 攻击提高 3
    @定义 暴击提高 2
    @终结技模板 begin
        @目标 全体辅助
        @效果 所有目标(技能) begin
            附加持续状态!(事件, 状态 = 攻击▲(事件, 数值 = 技能.攻击提高, 持续时间 = 2))
            附加持续状态!(事件, 状态 = 暴击▲(事件, 数值 = 技能.暴击提高, 持续时间 = 2))
        end
    end
end

@创建技能 被动 先人一步 begin
    @时机 进入战斗时
    @时机 消灭时
    @条件 begin
        事件.目标 == 角色 && @不显示
    end
    @效果 [事件.目标] begin
        if 事件 isa 进入战斗
            for 目标 in 使用者全队(技能, 事件)
                目标.穿甲 += 1
            end
        else
            for 目标 in 使用者全队(技能, 事件)
                目标.穿甲 -= 1
            end
        end
    end
end

@创建技能 被动 在旗帜下 begin
    @入场技能模板 begin
        @效果 使用者全队(技能, 事件) begin
            附加持续状态!(事件, 目标 = 目标, 状态 = 攻击▲(事件, 目标 = 目标, 数值 = 1, 持续时间 = 2))
            附加持续状态!(事件, 目标 = 目标, 状态 = 防御▲(事件, 目标 = 目标, 数值 = 1, 持续时间 = 2))
        end    
    end
end

@角色创建完成