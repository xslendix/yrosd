project('yrosd', 'c', 'cpp', default_options : [
  'c_std=c17',
  'cpp_std=c++20'
  ])

cc = meson.get_compiler('c')

dependencies = [
  dependency('toml', fallback : ['tomlc99', 'tomlc99_static_dep']),
  ]

sources = [
  'src/sysutil.c',
  'src/logging.c',
  'src/main.c',
  ]

executable('yrosd', sources, dependencies : dependencies)

