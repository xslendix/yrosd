project('yrosd', 'c', 'cpp', default_options : [
  'c_std=c17',
  'cpp_std=c++20'
  ])

cc = meson.get_compiler('c')

add_global_arguments(['-Wall', '-Wno-unknown-pragmas'], language : ['c', 'cpp'])


dependencies = [
  dependency('toml', fallback : ['tomlc99', 'tomlc99_static_dep']),
  dependency('libnm'),
  ]

sources = [
  'src/common.c',
  'src/networking.c',
  'src/logging.c',
  'src/server.c',
  'src/main.c',
  'src/pigpiod_if.c',
  'src/settings.c',
  'src/sysutil.c',
  ]

executable('yrosd', sources, dependencies : dependencies)

