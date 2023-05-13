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

hello_dependencies = []
hello_dependencies += fmt_dep
# hello_dependencies += threads_dep
# hello_dependencies += spdlog_dep

hello_compile_args = []
# hello_compile_args += '-DXXXX'

# 类似include机制
subdir('src')
subdir('example')
subdir('tests')
