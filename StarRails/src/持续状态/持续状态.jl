# 施加在角色身上的持续状态
abstract type 持续状态 end

@enum 持续状态定性 正面状态 负面状态 中性状态

include("附加持续状态.jl")
include("脱离持续状态.jl")
include("回合开始.jl")
include("回合结束.jl")

状态名称(状态::持续状态) = ""

macro 创建持续状态(状态名::Symbol, 状态定义::Expr)
    状态名字符串 = String(状态名)
    quote
        mutable struct $状态名 <: 持续状态
            战场::战场
            定性::持续状态定性
            来源::角色
            目标::角色
            持续时间::Int
            $(esc(状态定义))
        end

        $(esc(:状态名称))(状态::$状态名) = $状态名字符串
    end
end

Base.show(io::IO, 状态::持续状态) = print(io, 状态名称(状态))

