@创建角色 娜塔莎 begin
    属性 = 物理
    命途 = 丰饶
    生命 = 175
    能量 = 进度"1/3"
    技力 = 进度"0/0"
    攻击 = 8
    防御 = 6
    速度 = 10
    命中 = 0
    抵抗 = 3
    暴击 = 0
    穿甲 = 0
end

@创建技能 普攻 仁慈的背面 begin
    @标准普攻模板
end

@创建技能 战技 爱〆救护和抉择 begin
    @定义 治疗量 13.0
    @定义 复苏治疗量 10.0
    @战技模板 begin
        @目标 单体治疗
        @效果 所有目标(技能) begin
            治疗目标!(事件, 数值 = 技能.治疗量)
            解除负面状态!(事件)
            附加持续状态!(事件, 状态 = 复苏(事件, 目标 = 目标, 数值 = 技能.复苏治疗量, 持续时间 = 3))
        end
    end
end

@创建技能 终结技 新生之礼 begin
    @定义 治疗量 24.0
    @终结技模板 begin
        @目标 全体治疗
        @效果 所有目标(技能) begin
            治疗目标!(事件, 数值 = 技能.治疗量)
        end
    end
end

@创建技能 被动 生机焕发 begin
    @时机 治疗开始时
    @条件 begin
        事件.来源 == 角色 &&
        事件.目标.生命.当前 <= 事件.目标.生命.上限 * 0.3
    end
    @效果 [事件.目标] begin
        事件.造成治疗量 += 9 * 10
    end
end

@创建技能 被动 催眠研习 begin
    @入场技能模板 begin
        @效果 使用者敌队(技能, 事件) begin
            附加持续状态!(事件, 目标 = 目标, 状态 = 攻击▼(事件, 目标 = 目标, 数值 = 2), 命中 = 6)
        end
    end    
end

@角色创建完成