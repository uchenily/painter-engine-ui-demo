project('painter-engine-ui-demo', ['c', 'cpp'],
  version: '0.0.1',
  default_options: [
    'cpp_std=c++20',
    'cpp_args=-Wall -Wextra -Wpedantic -Wshadow'],
  meson_version: '>=1.0.0')

includes = include_directories(
  'PainterEngine/core',
  'PainterEngine/kernel',
  'PainterEngine/architecture',
  'PainterEngine/project',
  'PainterEngine/platform/modules',
  'PainterEngine')

freeglut_dep = dependency('freeglut')

hello_dependencies = []
hello_dependencies += freeglut_dep

hello_compile_args = []
hello_compile_args += '-DNDEBUG'

subdir('example')
