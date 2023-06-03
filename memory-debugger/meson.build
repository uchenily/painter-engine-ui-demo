project('debugger', ['c', 'cpp'],
  version: '0.0.1',
  default_options: [
    'cpp_std=c++20',
    'cpp_args=-Wall -Wextra -Wpedantic -Wshadow',
    'default_library=static'],
  meson_version: '>=1.0.0'
)


compiler = meson.get_compiler('c')
is_emscripten = compiler.get_id() == 'emscripten'
message('emscripten:', is_emscripten)

includes = include_directories(
  'project',
  'platform',
  '../PainterEngine')
  #'../PainterEngine/core',
  #'../PainterEngine/kernel',
  #'../PainterEngine/architecture')

c_files = run_command('find_all_c_file.sh', check: true)
painterengine_source_files = c_files.stdout().strip().split('\n')
painterengine_dependencies = []
painterengine_compile_args = []
  #'-s', 'EXPORTED_RUNTIME_METHODS="\["cwrap","ccall"\]"',
  #'-s', 'WEBSOCKET_SUBPROTOCOL="binary"',
  #'-s', 'SOCKET_DEBUG=1',
  #'-s', 'INITIAL_MEMORY=268435456',
  #'-s', 'USE_SDL',
  #'--preload-file', 'assets']
  #'--preload-file', 'platform/webassembly/assets']

painterengine_lib = library('painterengine',
  painterengine_source_files,
  include_directories: includes,
  dependencies: painterengine_dependencies,
  c_args: ['-s', 'USE_SDL'],
)

painterengine_dep = declare_dependency(
  include_directories: includes,
  #dependencies: painterengine_dependencies,
  link_with: painterengine_lib,
  compile_args: painterengine_compile_args
)

#emscripten_link_args = ['--bind', '-s', 'ALLOW_MEMORY_GROWTH=1', '-s', 'MODULARIZE=1', '--minify', '0', '-s', 'USE_SDL']
emscripten_link_args = [
  '-O3', # 必须用O3才能跑, 我也是服了. 为啥?
  #'-s', 'ALLOW_MEMORY_GROWTH=1',
  '-lwebsocket.js',
  # '-l', 'websocket.js', # 有问题 
  '-s', 'EXPORTED_RUNTIME_METHODS=["cwrap","ccall"]',
  '-s', 'WEBSOCKET_SUBPROTOCOL="binary"',
  '-s', 'SOCKET_DEBUG=1',
  '-s', 'INITIAL_MEMORY=268435456',
  '-s', 'USE_SDL',
  '--preload-file', 'assets']

executable(
  'PainterEngine',
  [
    meson.current_source_dir() / 'project/PainterEngine_Application.c',
    meson.current_source_dir() / 'project/PainterEngine_Startup.c',
  ],
  include_directories: includes,
  #name_suffix: 'js',
  name_suffix: 'html',
  dependencies: [painterengine_dep],
  link_args: emscripten_link_args
)
