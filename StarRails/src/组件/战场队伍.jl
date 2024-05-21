# 和队伍相关的战场函数

function 战技点(战场::战场, 角色::角色)::进度
    if 角色.队伍 == 蓝方
        return 战场.蓝方战技点
    elseif 角色.队伍 == 红方
        return 战场.红方战技点
    end
    return 进度"0/0"
end

function 战技点变化!(战场::战场; 角色::角色, 数值::Int)
    if 角色.队伍 == 蓝方
        战场.蓝方战技点 += 数值
    elseif 角色.队伍 == 红方
        战场.红方战技点 += 数值
    end
end

function 敌方队伍(战场::战场, 角色::角色)
    if 角色.队伍 == 蓝方
        return filter(目标 -> 目标.存活, 战场.红方角色)
    elseif 角色.队伍 == 红方
        return filter(目标 -> 目标.存活, 战场.蓝方角色)
    end
    return []
end

function 我方队伍(战场::战场, 角色::角色)
    if 角色.队伍 == 蓝方
        return filter(目标 -> 目标.存活, 战场.蓝方角色)
    elseif 角色.队伍 == 红方
        return filter(目标 -> 目标.存活, 战场.红方角色)
    end
    return []
end

function 左侧角色(战场::战场, 角色::角色)
    我方 = 我方队伍(战场, 角色)
    序号 = findlast(单位 -> 单位.位置 < 角色.位置, 我方)
    if 序号 === nothing
        return nothing
    end
    return 我方[序号]
end

function 右侧角色(战场::战场, 角色::角色)
    我方 = 我方队伍(战场, 角色)
    序号 = findfirst(单位 -> 单位.位置 > 角色.位置, 我方)
    if 序号 === nothing
        return nothing
    end
    return 我方[序号]
end

function 按照结算次序进行(战场::战场; 操作::Function)
    蓝方角色 = 战场.蓝方角色
    红方角色 = 战场.红方角色
    for i in 1:max(length(蓝方角色), length(红方角色))
        if i <= length(蓝方角色)
            操作(蓝方角色[i])
        end
        if i <= length(红方角色)
            操作(红方角色[i])
        end
    end
end