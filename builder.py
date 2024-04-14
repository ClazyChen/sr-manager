# 特别注意：
# 现在 cmake 对 C++20/23 中的 module 支持性不够
# 且存在需要手动安装 clang / gcc / ninja 等问题
# 因此，本项目要求用户不使用 module

import os
import sys
import re
import yaml

current_path = os.path.dirname(os.path.abspath(__file__))
template_cmake_path = os.path.join(current_path, "template", "CMakeLists.txt")

# 正常命令行，黄色字体
def print_normal(text, end='\n'):
    print('\033[1;33m' + text + '\033[0m', end=end)

# 错误命令行，红色字体
def print_error(text):
    print('\033[1;31m' + text + '\033[0m')

# 脚本命令行，绿色字体
def print_script(text, end='\n'):
    print('\033[1;32m' + text + '\033[0m', end=end)

script_open = False
def print_auto(text, end='\n'):
    global script_open
    if script_open:
        print_script(text, end)
    else:
        print_normal(text, end)

# 解析路径
def parse_path(path):
    global current_path
    # 绝对路径
    # linux
    if path[0] == '/':
        return path
    # windows
    elif path[1] == ':':
        return path
    # 回到本地目录
    elif path == '~':
        return os.path.dirname(os.path.abspath(__file__))
    # 相对路径
    else:
        return os.path.join(current_path, path)

# cd 命令，用于切换目录
def cd(path):
    global current_path
    current_path = parse_path(path)
    # 路径不存在
    if not os.path.exists(current_path):
        print_error('error: path not exists')
        current_path = os.path.dirname(current_path)
    # 路径是文件
    elif os.path.isfile(current_path):
        print_error('error: path is a file')
        current_path = os.path.dirname(current_path)
    # 路径是目录
    else:
        # 路径折叠
        current_path = os.path.abspath(current_path)


cpp_suffix = ['.cpp', '.cc', '.cxx', '.c++', '.cp', '.c', '.C']
header_suffix = ['.h', '.hpp', '.hxx', '.h++', '.hp', '.hh', '.H']

# 判断是否为 GB2312 编码，如果是，将其转换为 UTF-8 编码
def convert_to_utf8(path):
    # 读取文件内容
    with open(path, 'rb') as f:
        content = f.read()
    # 如果文件内容是 GB2312 编码，则转换为 UTF-8 编码
    try:
        content.decode('utf-8')
    except:
        content = content.decode('gb2312').encode('utf-8')
    # 写入文件
    with open(path, 'wb') as f:
        f.write(content)

# 在目标目录下，递归查找所有的 C++ 代码文件，并输出相对路径的列表
# 其中，out 文件夹下的是输出文件，不需要编译
def find_cpp_files(target_path):
    cpp_files = []
    header_files = []
    for root, dirs, files in os.walk(target_path):
        for file in files:
            # 如果是输出文件夹，跳过
            if os.path.relpath(root, target_path).startswith('out'):
                continue
            if os.path.splitext(file)[1] in cpp_suffix:
                convert_to_utf8(os.path.join(root, file))
                cpp_files.append(os.path.relpath(os.path.join(root, file), target_path))
            elif os.path.splitext(file)[1] in header_suffix:
                convert_to_utf8(os.path.join(root, file))
                header_files.append(os.path.relpath(os.path.join(root, file), target_path))
    return cpp_files, header_files

# build 命令，用于构建 CMakeLists.txt
# 指令格式：build [-p target_path] project_name
def build(*args):
    args = list(args)
    default_path = current_path
    # 解析参数
    target_path = None
    project_name = None
    for arg in args:
        if arg == '-p':
            target_path = parse_path(args[args.index(arg) + 1])
            args[args.index(arg) + 1] = None
        elif arg != None:
            project_name = arg
    if target_path == None:
        target_path = default_path
    # 如果目标目录是文件，报错
    if os.path.isfile(target_path):
        print_error('error: path is a file')
        return
    # 如果目标目录不存在，创建目录
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    cmake_path = os.path.join(target_path, "CMakeLists.txt")
    # 删除旧的 CMakeLists.txt
    if os.path.exists(cmake_path):
        print_auto('remove old CMakeLists.txt')
        os.remove(cmake_path)
    # 在目标目录下，递归查找所有的 C++ 源文件
    cpp_files, header_files = find_cpp_files(target_path)
    cpp_files_list = '\n'.join(['    "' + file + '"' for file in cpp_files])
    header_files_list = '\n'.join(['    "' + file + '"' for file in header_files])
    # 特别注意，Windows 下的路径分隔符是 \，但是 CMakeLists.txt 中的路径分隔符是 /，因此需要替换
    cpp_files_list = cpp_files_list.replace('\\', '/')
    header_files_list = header_files_list.replace('\\', '/')
    # 读取模板文件
    with open(template_cmake_path, 'r', encoding='utf-8') as f:
        template_cmake = f.read().format(project_name=project_name, cpp_files_list=cpp_files_list, header_files_list=header_files_list)
    # 写入 CMakeLists.txt
    with open(cmake_path, 'w', encoding='utf-8') as f:
        f.write(template_cmake)
    print_auto('generate CMakeLists.txt')

# 下面用于解析 yaml 文件
retraction = 0
def retraction_add():
    global retraction
    retraction += 1

def retraction_sub():
    global retraction
    retraction -= 1
    if retraction < 0:
        retraction = 0

def retraction_reset():
    global retraction
    retraction = 0

def fwrite(file, text):
    global retraction
    file.write('    ' * retraction + text)

# 用 yaml2 覆盖 yaml1 中的字段
def update(yaml1, yaml2):
    result = yaml1
    for key in yaml2:
        if key in result:
            if type(result[key]) == dict and type(yaml2[key]) == dict:
                result[key] = update(result[key], yaml2[key])
            else:
                result[key] = yaml2[key]
        else:
            result[key] = yaml2[key]
    return result

module_name = None
object_name = None
def parse_field(field):
    # field 是字段名
    # 格式 module1.module2.module3.field
    # 如果包含了 module_name，将其替换为 object_name
    if module_name != None and field.startswith(module_name):
        field = field[len(module_name)+1:]
        field = object_name + '.' + field
    field = field.replace('.', '::')
    return field

def parse_string(obj):
    # obj 是一个字符串表达式，其中可能包含 {字段} 形式的部分
    # 将所有 {字段} 中的字段进行解析、替换
    # 例如：{module1.module2.module3.field1} + {module1.module2.module3.field2}
    # 解析为：object_name.field1 + object_name.field2
    # 使用正则表达式提取出所有 {字段}，然后进行替换
    pattern = re.compile(r'\{([A-Za-z0-9_\.]+)\}')
    while pattern.search(obj) != None:
        # 提取出 {字段}
        field = pattern.search(obj).group(1)
        # 将 {字段} 替换为 object_name.field
        obj = obj.replace('{' + field + '}', parse_field(field))
    obj = obj.replace('$', '')
    return obj

def object_to_string(obj):
    if type(obj) == str:
        return '\"' + obj + '\"'
    elif type(obj) == bool:
        return 'true' if obj else 'false'
    else:
        return str(obj)

def fwrite_object(file, objname, obj):
    if type(obj) == dict:
        fwrite(file, "struct {objname} {{\n".format(objname=objname))
        retraction_add()
        for key in obj:
            fwrite_object(file, key, obj[key])
        retraction_sub()
        fwrite(file, "};\n")
    elif type(obj) == list or type(obj) == tuple:
        fwrite(file, "static constexpr std::array {objname} = {{ {values} }};\n".format(objname=objname, values=', '.join(map(object_to_string, obj))))
    elif type(obj) == str:
        if obj[0] == '$': # 表达式
            fwrite(file, "static constexpr auto {objname} = {value};\n".format(objname=objname, value=parse_string(obj[1:])))
        else: # 普通字符串
            fwrite(file, "static constexpr const char* {objname} = {value};\n".format(objname=objname, value=object_to_string(obj)))
    elif type(obj) == bool:
        fwrite(file, "static constexpr bool {objname} = {value};\n".format(objname=objname, value=object_to_string(obj)))
    else: # int
        # add "ll" suffix to the end of long long
        if obj > 2147483647 or obj < -2147483648:
            fwrite(file, "static constexpr long long {objname} = {value};\n".format(objname=objname, value=object_to_string(obj) + 'll'))
        else:
            fwrite(file, "static constexpr int {objname} = {value};\n".format(objname=objname, value=object_to_string(obj)))
    
# config 指令
# 指令格式：config [-n namespace] [-p target_path] [-m yaml_module] [-o output_file] yaml_file yaml_file2 yaml_file3 ...
# 将 yaml 文件转换为 C++ 配置文件，语法和硬件（scala）配置文件相同
# 如果指定了不止一个 yaml 文件，生成结果将是所有这些 yaml 文件中配置的字段的并集（后面的 yaml 文件中的字段会覆盖前面的）
def config(*args):
    global module_name
    global object_name
    args = list(args)
    default_namespace_name = 'fpga'
    default_output_file = 'Config.hpp'
    # 解析参数
    namespace_name = None
    target_path = current_path
    output_file = None
    module_name = None
    yaml_file = []
    for arg in args:
        if arg == '-n':
            namespace_name = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg == '-p':
            target_path = parse_path(args[args.index(arg) + 1])
            args[args.index(arg) + 1] = None
        elif arg == '-m':
            module_name = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg == '-o':
            output_file = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg != None:
            yaml_file.append(parse_path(arg))
    if namespace_name == None:
        namespace_name = default_namespace_name
    if output_file == None:
        output_file = default_output_file
    # 如果目标目录是文件，报错
    if os.path.isfile(target_path):
        print_error('error: path is a file')
        return
    # 如果目标目录不存在，创建目录
    if not os.path.exists(target_path):
        os.makedirs(target_path)
    # 保证 output_file 是 C++ 头文件，否则添加 .hpp 后缀
    if os.path.splitext(output_file)[1] not in header_suffix:
        output_file += '.hpp'
    # output_file 只允许包含字母 数字 下划线 和头文件后缀（header_suffix），且必须以字母开头
    output_file_name, output_file_suffix = os.path.splitext(output_file)
    if not re.match(r'^[a-zA-Z][a-zA-Z0-9_]*$', output_file_name) or output_file_suffix not in header_suffix:
        print_error('error: invalid output file name')
        return
    # output_file 去掉 头文件后缀 的部分，作为顶层 struct 的名字
    object_name = os.path.splitext(output_file)[0]
    # 保证 yaml 文件存在
    if len(yaml_file) == 0:
        print_error('error: no yaml file')
        return
    for file in yaml_file:
        if not os.path.exists(file):
            print_error('error: yaml file {file_name} not exists'.format(file_name=file))
            return
    # 读取 yaml 文件
    yaml_contents = []
    for file in yaml_file:
        with open(file, 'r', encoding='utf-8') as f:
            try:
                yaml_contents.append(yaml.load(f.read(), Loader=yaml.FullLoader))
            except:
                print_error('error: yaml file {file_name} format error'.format(file_name=file))
                return
    # 将所有的 yaml 文件合并，后面的 yaml 文件中的字段会覆盖前面的
    yaml_content = yaml_contents[0]
    for content in yaml_contents[1:]:
        yaml_content = update(yaml_content, content)
    # 如果指定了 module_name，则只转换制定的模块
    if module_name != None:
        module_levels = module_name.split('.')
        for level in module_levels:
            if level not in yaml_content:
                print_error('error: module {module_name} not exists'.format(module_name=module_name))
                return
            yaml_content = yaml_content[level]
    # 在目标目录下生成 C++ 配置文件
    output_file_path = os.path.join(target_path, output_file)
    retraction_reset()
    with open(output_file_path, "w", encoding='utf-8') as f:
        fwrite(f, "#pragma once\n\n")
        fwrite(f, "#include <array>\n\n")
        fwrite(f, "namespace {namespace_name} {{\n".format(namespace_name=namespace_name))
        retraction_add()
        fwrite_object(f, object_name, yaml_content)
        retraction_sub()
        fwrite(f, "}\n")
    # 转换成功
    print_auto('success: generate {output_file}'.format(output_file=output_file))

# test 指令
# 用于选用一个 template 文件夹下的模板文件，生成测试用的 .cpp 文件
# 指令格式：test [-p target_path] [-m module] [-n namespace] [-o output_file] template_file
def test(*args):
    args = list(args)
    # 默认生成的 .cpp 文件名为当前目录名（首字母大写）
    default_output_file = os.path.basename(current_path).capitalize() + '.cpp'
    # 默认的顶层命名空间名为 fpga
    default_namespace_name = 'fpga'
    # 默认的目标目录为当前目录
    default_target_path = current_path
    # 解析参数
    output_file = None
    namespace_name = None
    target_path = None
    module_name = None
    template_file = None
    include_paths = []
    for arg in args:
        if arg == '-p':
            target_path = parse_path(args[args.index(arg) + 1])
            args[args.index(arg) + 1] = None
        elif arg == '-m':
            module_name = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg == '-n':
            namespace_name = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg == '-o':
            output_file = args[args.index(arg) + 1]
            args[args.index(arg) + 1] = None
        elif arg == '-I':
            include_paths = args[args.index(arg) + 1].split(':')
            args[args.index(arg) + 1] = None
        elif arg != None:
            template_file = parse_path(arg)
    # 如果模板文件不存在或者是目录，报错
    if template_file == None:
        print_error('error: no template file')
        return
    if not os.path.exists(template_file):
        print_error('error: template file not exists')
        return
    if not os.path.isfile(template_file):
        print_error('error: template file is not a file')
        return
    # 如果目标目录不存在或者是文件，报错
    if target_path == None:
        target_path = default_target_path
    if not os.path.exists(target_path):
        print_error('error: target path not exists')
        return
    if os.path.isfile(target_path):
        print_error('error: target path is a file')
        return
    if namespace_name == None:
        namespace_name = default_namespace_name
    # module_name 格式检查
    # module_name 只允许包含字母 数字 下划线 和点，且必须以字母开头
    if module_name is None:
        print_error('error: no module name')
        return
    if not re.match(r'^[a-zA-Z][a-zA-Z0-9_.]*$', module_name):
        print_error('error: invalid module name')
        return
    # module_name 示例： test.testmat.TestGetKey
    # 将其转换为：
    # test_module_path = 'test/testmat/TestGetKey.hpp'
    # test_module_name = 'test::testmat::TestGetKey'
    module_name_levels = module_name.split('.')
    module_name = '::'.join(module_name_levels)
    # 检查 include_paths
    # 保证 target_path / module_path 是存在的 C++ 头文件
    for include_path in include_paths:
        full_path = os.path.join(target_path, include_path)
        if not os.path.exists(full_path):
            print_error('error: include path {include_path} not exists'.format(include_path=include_path))
            return
        if not os.path.isfile(full_path):
            print_error('error: include path {include_path} is not a file'.format(include_path=include_path))
            return
    # 将 include_paths 转换为 C++ 头文件包含语句
    include_path = '\n'.join(['#include "{include_path}"'.format(include_path=include_path) for include_path in include_paths])
    # 如果 output_file 没有指定后缀，则添加 .cpp 后缀
    if output_file == None:
        output_file = default_output_file
    if os.path.splitext(output_file)[1] not in cpp_suffix:
        output_file += '.cpp'
    # 将模板文件复制到目标目录下
    with open(template_file, 'r', encoding='utf-8') as f:
        template_content = f.read().format(module_name=module_name, namespace_name=namespace_name, include_path=include_path)
    with open(os.path.join(target_path, output_file), 'w', encoding='utf-8') as f:
        f.write(template_content)
    # 生成成功
    print_auto('success: generate {output_file}'.format(output_file=output_file))

# 查看帮助
def help():
    print_auto('''
    cd [path]       切换目录
    ls              查看当前目录下的所有文件和文件夹
    build [project] 构建 CMakeLists.txt
        -p [path]   指定目标目录
    config [yaml]   将 yaml 配置文件转换为 C++ 配置文件
        -n [name]   指定顶层命名空间名
        -p [path]   指定目标目录
        -m [module] 指定转换的模块
        -o [struct] 指定生成的 C++ 静态结构体名
    test [target]   选用模板文件生成测试的 .cpp 文件
        -p [path]   指定目标目录
        -m [module] 指定被测试的模块名
        -n [name]   指定顶层命名空间名
        -o [output] 指定生成的 .cpp 文件名
    run             运行指定的脚本文件
    ![syscall]      执行系统命令
        (e.g.) !python  启动 python 交互式命令行
    help            查看帮助
    exit            退出
    ''')

# 运行指定的脚本文件
def run(path):
    global script_open
    path = parse_path(path)
    if not os.path.exists(path):
        print_error('error: script file not exists')
        return
    if not os.path.isfile(path):
        print_error('error: script file is not a file')
        return
    # 读入脚本文件并执行
    with open(path, 'r') as f:
        lines = f.readlines()
    script_open = True
    for line in lines:
        line = line.strip()
        if len(line) == 0 or line.startswith('#'):
            continue
        print_auto(" > " + line)
        execute(line)
    script_open = False

# 查看当前目录下的所有文件和文件夹
def ls():
    for file in os.listdir(current_path):
        print_auto(file)

# 退出
def exit():
    import sys
    sys.exit(0)

# 指令集字典
command_dict = {
    'cd': cd,
    'exit': exit,
    'quit': exit,
    'build': build,
    'config': config,
    'test': test,
    'run': run,
    'ls': ls,
    'help': help
}

# 执行指令
def execute(command):
    # 分割指令
    command = command.split(' ')
    # 去除空指令
    command = [x for x in command if x != '']
    # 指令不存在
    if len(command) == 0:
        return
    # 获取指令
    command_name = command[0]
    # 获取参数
    command_args = command[1:]
    # 执行指令
    if command_name in command_dict:
        #try:
        command_dict[command_name](*command_args)
        #except Exception as e:
        #    print_error('error: ' + str(e))
    elif command_name.startswith('!'): # 执行系统命令
        temp_path = os.getcwd()
        os.chdir(current_path)
        os.system(' '.join(command)[1:])
        os.chdir(temp_path)
    elif command_name.startswith('#'): # 注释
        print_normal(' '.join(command)) # 打印注释
    else:
        print_error('error: command {command_name} not exists'.format(command_name=command_name))

def main():
    while True:
        # 打印当前目录
        print_auto(current_path + ' > ', end='')
        # 输入一行指令
        command = input()
        # 执行指令
        execute(command)

if __name__ == '__main__':
    # 如果没有参数，则进入交互式命令行
    if len(sys.argv) == 1:
        main()
    # 否则执行指定的脚本文件
    else:
        run(sys.argv[1])