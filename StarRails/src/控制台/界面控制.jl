# 界面控制相关的开关

mutable struct 界面控制
    开启打印战场信息::Bool
    开启打印战斗过程::Bool
    开启打印速度条::Bool
    开启等待用户输入::Bool
    界面控制() = new(true, true, true, true)
end

const 当前界面控制 = 界面控制()

function 隐藏所有界面()
    当前界面控制.开启打印战场信息 = false
    当前界面控制.开启打印战斗过程 = false
    当前界面控制.开启打印速度条 = false
    当前界面控制.开启等待用户输入 = false
end

function 显示所有界面()
    当前界面控制.开启打印战场信息 = true
    当前界面控制.开启打印战斗过程 = true
    当前界面控制.开启打印速度条 = true
    当前界面控制.开启等待用户输入 = true
end

function 控制台打印(信息)
    if 当前上下文.是否不显示下一条信息
        当前上下文.是否不显示下一条信息 = false
        return
    end
    当前界面控制.开启打印战斗过程 && print(信息)
end
控制台换行(信息) = 控制台打印(信息 * "\n")
控制台换行() = 控制台打印("\n")

等待用户输入() = 当前界面控制.开启等待用户输入 && readline()
