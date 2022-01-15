set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER __TOOLPATH__/cross/toolchain/x-tools/armv8-rpi3-linux-gnueabihf/bin/armv8-rpi3-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER __TOOLPATH__/cross/toolchain/x-tools/armv8-rpi3-linux-gnueabihf/bin/armv8-rpi3-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH __TOOLPATH__/cross/toolchain/sysroot-armv8-rpi3-linux-gnueabihf)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
add_definitions(-Wall -Wno-unknown-pragmas -Wno-psabi)
