# 项目定义
project('cpp_template', 'cpp',
  version: '0.0.1',
  default_options: [
    'cpp_std=c++20',
    'cpp_args=-Wall -Wextra -Wpedantic -Wshadow'],
  meson_version: '>=1.0.0')

# 头文件搜索目录
includes = include_directories('include')

# 外部第三方依赖
# threads_dep = dependency('threads')
fmt_dep = dependency('libfmt',
  # 先从系统中找, 没找到fallback到subprojects中找
  fallback: ['fmt', 'fmt_dep'],
  default_options: ['default_library=static'])

# 如果是外部库, 使用这种方式
#hello_dep = dependency('libhello',
#  fallback: ['hello', 'hello_dep'],
#  default_options: ['default_library=static'])


hello_dependencies = []
hello_dependencies += fmt_dep
# hello_dependencies += threads_dep
# hello_dependencies += spdlog_dep

hello_compile_args = []
# hello_compile_args += '-DXXXX'

# 类似include机制
# src下定义了hello_lib, 用于下面的链接
# 可以根据meson_options.txt中定义的选项compile_library来判断是否需要编译成库
# if get_option('compile_library')
#   ...
# else
#   ...
# endif
#
# 在default_options中定义选项值: default_options: ['compile_library=true']

# 如果是内部库, 使用这种方式
subdir('src')

# 内部声明一个依赖(链接为static/shared库)
hello_dep = declare_dependency(
  include_directories: includes,
  dependencies: hello_dependencies,
  link_with: hello_lib,
  compile_args: hello_compile_args)

hello_dependencies += hello_dep

# 生成可执行文件
exe = executable('cpp_template', 'example/main.cpp',
  include_directories: includes,
  dependencies: hello_dependencies)

# 测试
test('basic', exe)
