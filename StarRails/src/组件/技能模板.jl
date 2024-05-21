# 提供经典的普攻、战技和终结技模板

macro 普攻模板(效果::Expr)
    quote
        $效果
        @效果 使用者(技能) begin
            获得战技点!(事件)
            获得能量!(事件)
        end
    end
end

macro 标准普攻模板()
    quote
        @定义 威力 常数.标准普攻威力
        @定义 削韧 常数.标准普攻削韧
        @普攻模板 begin
            @单体攻击
        end
    end
end

macro 战技模板(效果::Expr)
    quote
        @定义 削韧 常数.标准战技削韧
        @定义 主目标削韧 常数.标准战技削韧
        @定义 副目标削韧 常数.标准战技副目标削韧
        @条件 begin
            持有战技点数量(事件) >= 1
        end
        @效果 使用者(技能) begin
            消耗战技点!(事件)
        end
        $效果
        @效果 使用者(技能) begin
            获得能量!(事件)
        end
    end
end

macro 终结技模板(效果::Expr)
    quote
        @定义 削韧 常数.标准终结技削韧
        @定义 主目标削韧 常数.标准终结技削韧
        @定义 副目标削韧 常数.标准终结技副目标削韧
        @条件 begin
            角色.能量.当前 == 角色.能量.上限
        end
        @效果 使用者(技能) begin
            消耗能量!(事件)
        end
        $效果
    end
end

macro 入场技能模板(效果::Expr)
    quote
        @时机 进入战斗时
        @条件 begin
            事件.目标 == 角色
        end
        $效果 
    end
end