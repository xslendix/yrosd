set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER __TOOLPATH__/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/bin/arm-rpi-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER __TOOLPATH__/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/bin/arm-rpi-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH __TOOLPATH__/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/arm-linux-gnueabihf/sysroot)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
add_definitions(-Wall -std=c11)
