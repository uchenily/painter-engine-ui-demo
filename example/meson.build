# 生成可执行文件

sources = []
#sources += ['px_file.cpp', 'px_display.cpp']
c = run_command('find_all_c_file.sh', check: true)
sources += c.stdout().strip().split('\n')


exe = executable('demo',
  sources,
  include_directories: includes,
  dependencies: hello_dependencies)
