# 发动攻击，核心机制

@创建事件 发动攻击 begin
    
    威力::Int
    属性::标签.属性

    来源攻击::Int
    来源暴击::Int
    来源穿甲::Int
    目标防御::Int

    造成伤害::Int
    造成暴击::Bool
    造成穿甲::Bool
    被护盾抵消的伤害::Int

    function 发动攻击(战场::战场; 来源::角色, 目标::角色, 威力::Int, 性质::Vector{标签.性质}=[])
        new(战场, 性质, 来源, 目标, 威力, 来源.模板.属性, 来源.攻击, 来源.暴击, 来源.穿甲, 目标.防御, 0, false, false, 0)
    end

end

@触发事件 begin
    触发时机!(事件, 攻击开始时)    

    事件.造成暴击 = 随机判定(事件.来源暴击)
    事件.造成穿甲 = 随机判定(事件.来源穿甲)
    事件.造成伤害 = 事件.来源攻击 * 事件.威力 ÷ 10
    if !事件.造成穿甲 || 事件.目标防御 < 0
        事件.造成伤害 -= 事件.目标防御 * 10
    end
    if 事件.造成暴击
        事件.造成伤害 = 事件.造成伤害 * 3 ÷ 2
    end
    事件.造成伤害 = max(事件.造成伤害, 1)

    事件.被护盾抵消的伤害 = 0
    for 状态 in 事件.目标.状态列表
        if 状态 isa 护盾
            if 状态.数值 <= 事件.造成伤害
                事件.被护盾抵消的伤害 = max(事件.被护盾抵消的伤害, 状态.数值)
                状态.数值 = 0
                脱离持续状态!(事件, 状态 = 状态)
            else
                事件.被护盾抵消的伤害 = 事件.造成伤害
                状态.数值 -= 事件.造成伤害
            end
        end
    end
    filter!(状态 -> !(状态 isa 护盾) || 状态.数值 > 0, 事件.目标.状态列表)
    事件.造成伤害 -= 事件.被护盾抵消的伤害

    控制台换行("$事件")

    if 事件.造成伤害 > 0
        扣减生命!(事件, 数值 = 事件.造成伤害)
    end

    触发时机!(事件, 攻击结束时)
end

function Base.show(io::IO, 事件::发动攻击)
    暴击穿甲说明 = ""
    if 事件.造成暴击 && 事件.造成穿甲
        暴击穿甲说明 = @绿 "暴击且穿甲！"
    elseif 事件.造成暴击
        暴击穿甲说明 = @绿 "暴击！"
    elseif 事件.造成穿甲
        暴击穿甲说明 = @绿 "穿甲！"
    end
    特殊伤害说明 = ""
    for 性质 in 事件.性质列表
        if 性质 in keys(特殊攻击显示)
            特殊伤害说明 *= "（$(特殊攻击显示[性质])）"
        end
    end
    护盾抵消说明 = ""
    if 事件.被护盾抵消的伤害 > 0
        护盾抵消说明 = "（护盾吸收 $(事件.被护盾抵消的伤害 / 10.0) 点） "
    end
    print(io, "【$(事件.来源)】对【$(事件.目标)】发动攻击，造成了 $(事件.造成伤害 / 10.0) 点 $(事件.属性) 伤害。$特殊伤害说明$暴击穿甲说明$护盾抵消说明")
end

function 攻击目标!(事件::事件; 威力::Int, 来源 = 事件.来源, 目标 = 事件.目标, 性质::Vector = [])
    触发(发动攻击(事件.战场, 来源 = 来源, 目标 = 目标, 威力 = 威力, 性质 = [事件.性质列表..., 性质...]))
end
