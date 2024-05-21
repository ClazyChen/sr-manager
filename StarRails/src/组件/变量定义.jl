# 在技能解析器中定义变量

macro 定义(变量名::Symbol, 默认值)
    解析器.私有变量列表[变量名] = eval(默认值)
    nothing
end
