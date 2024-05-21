# 在技能解析器中记录发动条件

macro 条件(判别式::Expr)
    解析器.判定条件 = 判别式
    nothing
end
