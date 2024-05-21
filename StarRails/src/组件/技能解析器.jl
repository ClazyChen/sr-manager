# 用于技能解析

mutable struct 技能解析器

    默认性质列表::Array{标签.性质}
    私有变量列表::Dict{Symbol, Any}
    对目标的效果::Expr
    已解析到生效::Bool
    生效前流程::Array{Expr}
    生效后流程::Array{Expr}
    触发时机::Array{标签.触发时机}
    判定条件::Any

    function 技能解析器()
        new([], Dict(), Expr(:block), false, [], [], [], true)
    end

end

const 解析器 = 技能解析器()

function 重置解析器(技能类型::标签.性质)
    解析器.默认性质列表 = [技能类型]
    解析器.私有变量列表 = Dict()
    解析器.对目标的效果 = Expr(:block)
    解析器.已解析到生效 = false
    解析器.生效前流程 = []
    解析器.生效后流程 = []
    解析器.触发时机 = []
    解析器.判定条件 = true
end

const 技能定义关键字::Vector{Symbol} = []