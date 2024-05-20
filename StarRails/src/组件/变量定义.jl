
macro 定义(变量名::Symbol, 默认值)
    if 默认值 isa Expr
        解析器.私有变量列表[变量名] = eval(默认值)
    else
        解析器.私有变量列表[变量名] = 默认值
    end
end

push!(技能定义关键字, Symbol("@定义"))