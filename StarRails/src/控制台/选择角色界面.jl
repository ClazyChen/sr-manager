# 通过用户交互选择双方的角色
最大单位数 = 1

function 选择角色(队伍::标签.队伍)
    println(@白 "选择 $队伍 的角色")
    被选中的角色列表::Vector{角色} = []
    for i in eachindex(角色数据库)
        println(@白 "($i) $(角色数据库[i].名称)")
    end
    for i in 1:最大单位数
        角色序号 = parse(Int, readline())
        if 角色序号 < 1 || 角色序号 > length(角色数据库)
            println(@红 "输入错误，请重新输入")
            i -= 1
        else
            println(@黄 "选择了 $(角色数据库[角色序号].名称)")
            push!(被选中的角色列表, 角色(
                模板 = 角色数据库[角色序号],
                队伍 = 队伍,
                位置 = i
            ))
        end
    end
    return 被选中的角色列表
end

function 运行()
    println("测试选择角色界面")
    println("输入最大单位数")
    global 最大单位数 = parse(Int, readline())
    蓝方角色 = 选择角色(蓝方)
    红方角色 = 选择角色(红方)
    战场(蓝方角色, 红方角色)
end
