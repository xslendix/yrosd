project('yrosd', 'c', 'cpp', default_options : [
  'c_std=c17',
  'cpp_std=c++20'
  ])

cc = meson.get_compiler('c')

add_global_arguments(['-Wall', '-Wno-unknown-pragmas'], language : ['c', 'cpp'])

dependencies = [
  dependency('toml', fallback : ['tomlc99', 'tomlc99_static_dep']),
  dependency('libnm'),
  dependency('openssl'),
  ]

sources = [
  'src/common.c',
  'src/motor_controller.c',
  'src/networking.c',
  'src/logging.c',
  'src/server.c',
  'src/settings.c',
  'src/sysutil.c',
  'src/yrosd.c',
  ]

if get_option('debugpigpio').enabled()
  sources += 'src/pigpiod_if2.c'
else
  dependencies += dependency('pigpio_if2')
endif

executable('yrosd', sources, dependencies : dependencies)

