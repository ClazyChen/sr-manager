# 用于技能解析

mutable struct 技能解析器

    默认性质列表::Vector{标签.性质}
    私有变量列表::Dict{Symbol, Any}
    效果列表::Vector{Expr}
    触发时机::Vector{标签.触发时机}
    是否显式指定目标::Bool
    显式指定目标方法::Expr
    判定条件::Any

    function 技能解析器()
        new([], Dict(), [], [], false, Expr(:block), true)
    end

end

const 解析器 = 技能解析器()

function 重置解析器(技能类型::标签.性质)
    解析器.默认性质列表 = [技能类型]
    解析器.私有变量列表 = Dict()
    解析器.效果列表 = []
    解析器.触发时机 = []
    解析器.是否显式指定目标 = false
    解析器.显式指定目标方法 = Expr(:block)
    解析器.判定条件 = true
end
