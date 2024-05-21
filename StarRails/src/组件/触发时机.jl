# 在技能解析器中定义触发时机

macro 时机(时机名::Symbol)
    push!(解析器.触发时机, eval(时机名))
    nothing
end
