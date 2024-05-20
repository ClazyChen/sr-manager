# 速度条，战场的重要组成元件，用于决定行动顺序

mutable struct 速度条节点
    单位::角色
    下次行动的时间::Float64
    是额外回合::Bool
    速度条节点(战斗单位::角色, 下次行动的时间::Float64; 是额外回合::Bool = false) = new(战斗单位, 下次行动的时间, 是额外回合)
end

# 比较两个节点的行动顺序
function Base.isless(左::速度条节点, 右::速度条节点)
    if 左.下次行动的时间 == 右.下次行动的时间
        return 行动次序(左.单位) < 行动次序(右.单位)
    end
    return 左.下次行动的时间 < 右.下次行动的时间
end

mutable struct 速度条
    全局时间::Float64
    节点::Vector{速度条节点}
    速度条() = new(0.0, [])
end

时间推进!(速度条::速度条) = (速度条.全局时间 = 速度条.节点[1].下次行动的时间)
当前角色(速度条::速度条)::角色 = 速度条.节点[1].单位

function 插入单位!(速度条::速度条, 单位::角色, 下次行动的时间::Float64; 是额外回合::Bool = false)
    新节点 = 速度条节点(单位, 下次行动的时间, 是额外回合 = 是额外回合)
    插入位置 = findfirst(节点 -> isless(新节点, 节点), 速度条.节点)
    if 插入位置 === nothing
        push!(速度条.节点, 新节点)
    else
        insert!(速度条.节点, 插入位置, 新节点)
    end
end

插入单位!(速度条::速度条, 单位::角色) = 插入单位!(速度条, 单位, 速度条.全局时间 + 行动间隔(单位))

function 删除单位!(速度条::速度条, 单位::角色)
    filter!(节点 -> 节点.单位 != 单位, 速度条.节点)
end

function 推条!(速度条::速度条, 单位::角色, 时间::Float64)
    位置 = findfirst(节点 -> 节点.单位 == 单位, 速度条.节点)
    被推条节点 = 速度条.节点[位置]
    被推条节点.下次行动的时间 += 时间
    deleteat!(速度条.节点, 位置)
    插入单位!(速度条, 被推条节点.单位, 被推条节点.下次行动的时间)
end

function 变速!(速度条::速度条, 单位::角色, 之前的间隔::Float64, 之后的间隔::Float64)
    位置 = findfirst(节点 -> 节点.单位 == 单位, 速度条.节点)
    被变速节点 = 速度条.节点[位置]
    被变速节点.下次行动的时间 += (被变速节点.下次行动的时间 - 速度条.全局时间) * (之后的间隔 - 之前的间隔) / 之前的间隔
    deleteat!(速度条.节点, 位置)
    插入单位!(速度条, 被变速节点.单位, 被变速节点.下次行动的时间)
end

插入额外回合!(速度条::速度条, 单位::角色) = 插入单位!(速度条, 单位, 速度条.全局时间, 是额外回合 = true)

function 更新!(速度条::速度条)
    当前节点 = 速度条.节点[1]
    deleteat!(速度条.节点, 1)
    !当前节点.是额外回合 && 插入单位!(速度条, 当前节点.单位)
end
