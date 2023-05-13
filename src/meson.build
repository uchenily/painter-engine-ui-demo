# 编译库所需的源文件
source_files = files(
  'hello.cpp',
)

# 定义一个库
hello_lib = library('hello',
  source_files, # 上级meson.build中定义的变量
  include_directories: includes, # 同上
  dependencies: hello_dependencies, # 同上
  cpp_args: hello_compile_args) # 同上

# 如果这是一个给其他的项目使用的第三方库, 则使用这种方式, 导出hello_dep这个依赖名
## 内部声明一个依赖(链接为static/shared库)
#hello_dep = declare_dependency(
#  include_directories: includes,
#  dependencies: hello_dependencies,
#  link_with: hello_lib,
#  compile_args: hello_compile_args)
#
# 然后把这个目录放到subprojects下, 供主项目使用. 主项目的meson.build中使用dependency加入进来
#hello_dep = dependency('libhello',
#  fallback: ['hello', 'hello_dep'],
#  default_options: ['default_library=static'])

