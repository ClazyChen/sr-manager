# 施加在角色身上的持续状态
abstract type 持续状态 end

@enum 持续状态定性 正面状态 负面状态 中性状态

include("附加持续状态.jl")
include("脱离持续状态.jl")
include("回合开始.jl")
include("回合结束.jl")

macro 创建持续状态(状态名::Symbol, 状态定义::Expr)
    quote
        mutable struct $状态名 <: 持续状态
            战场::战场
            定性::持续状态定性
            来源::角色
            目标::角色
            持续时间::Int
            $(esc(状态定义))
        end
    end
end

include("裂伤.jl")

