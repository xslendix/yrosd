#!/usr/bin/bash

LOG() {
    echo -e "\e[31m :: \e[0m$@"
}

if ! command -v cmake &> /dev/null; then
    LOG "CMake could not be found. Please install it."
    exit
fi

if ! command -v docker &> /dev/null; then
    LOG "Docker could not be found. Please install it."
    exit
fi

if id -nG "$(whoami)" | grep -qw "docker"; then
    LOG "Docker properly installed!"
else
    LOG "Please add your user to the docker group."
fi

if ! command -v ninja &> /dev/null; then
    LOG "Ninja could not be found. Please install it."
    exit
fi

if ! command -v python3 &> /dev/null; then
    LOG "Python 3 could not be found. Please install it."
    exit
fi

if ! command -v pip3 &> /dev/null; then
    LOG "Python 3 pip could not be found. Please install it."
    exit
fi

grep -q BCM2708 /proc/cpuinfo
TOOLCHAIN_ENABLED=$?
LOG "Toolchain enabled: $TOOLCHAIN_ENABLED"

set -e

toolchain() {
    if [ $TOOLCHAIN_ENABLED -ne 0 ] ; then
        if [ ! -f "toolchain-rpi.cmake" ] ; then
            LOG "Creating toolchain file"
            sed "s|__TOOLPATH__|$PWD/|g" toolchain-rpi.template.cmake > toolchain-rpi.cmake
        fi

        if [ ! -d "$PWD/cross/toolchain/x-tools" ] ; then
            LOG "Getting toolchain"
            git clone --depth 1 https://github.com/xslendix/RPi-Cpp-Toolchain cross
            cd cross/toolchain
            ./toolchain.sh rpi3-armv8 --export
            cd ../..
        fi
    else
        LOG "WARNING! BUILDING ON THE RASPBERRY PI IS NOT YET SUPPORTED!"
    fi
}

wiringpi () {
    if [ ! -f "vendor/WiringPi/.patched" ] ; then
        LOG "Getting WiringPi"
        git clone --depth 1 https://github.com/WiringPi/WiringPi vendor/WiringPi

        LOG "Patching WiringPi"

        LOG "Creating CMakeLists for WiringPi"
        cat<< EOF > vendor/WiringPi/wiringPi/CMakeLists.txt
cmake_minimum_required(VERSION 3.0)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "-std=gnu99 -D_POSIX_C_SOURCE=199309L -D_DEFAULT_SOURCE \${CMAKE_C_FLAGS}")

FILE(GLOB SRC_FILES *.c)

add_library(wiringPi SHARED \${SRC_FILES})

target_include_directories(wiringPi PUBLIC \${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(wiringPi \${CMAKE_THREAD_LIBS_INIT} crypt m rt)

EOF

    LOG "Copying toolchian cmake fie for WiringPi"
    cp toolchain-rpi.cmake vendor/WiringPi/wiringPi

    LOG "Compiling WiringPi"
    cd vendor/WiringPi/wiringPi
    if [ $TOOLCHAIN_ENABLED -ne 0 ] ; then
        cmake . -DCMAKE_TOOLCHAIN_FILE=toolchain-rpi.cmake -GNinja
    else
        cmake . -GNinja
    fi
    ninja
    cd ../../..

    touch "vendor/WiringPi/.patched"

    fi
}

cleantoolchain() {
    LOG "Deleting toolchain"
    sudo rm -rf cross toolchain-rpi.cmake
}

cleanwiringpi() {
    LOG "Deleting WiringPi"
    rm -rf vendor/WiringPi
}

clean() {
    LOG "Deleting build files"
    rm -rf build
}

purge() {
    clean
    cleanwiringpi
    cleantoolchain
}

build() {
    toolchain
    wiringpi

    if [ ! -d build ] ; then
        LOG "Creating build directory"
        mkdir -p build
    fi

    cd build

    if [ ! -f CMakeCache.txt ] ; then
        LOG "Generating build files"
        if [ $TOOLCHAIN_ENABLED -ne 0 ] ; then
            cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake -GNinja
        else
            cmake .. -GNinja
        fi
    fi

    LOG "Starting build"
    ninja

    cd ..
}

deploy() {
    build

    LOG "Grabbing settings"
    source settings.example.sh
    source settings.sh

    LOG "Deploying executable"
    scp -P$RPI_PORT build/dripd dripd.conf dripd.service $RPI_USER@$RPI_HOST:~
    # scp -P$RPI_PORT dripd.conf $RPI_USER@$RPI_HOST:~/dripd.conf
    # scp -P$RPI_PORT dripd.service $RPI_USER@$RPI_HOST:~/dripd.service
    ssh -p$RPI_PORT $RPI_USER@$RPI_HOST 'chmod +x ~/dripd && sudo cp -v ~/dripd /usr/bin && sudo cp -vn ~/dripd.conf /etc && sudo cp -v ~/dripd.service /etc/systemd/system && sudo systemctl restart dripd ; systemctl status dripd'
}

all() {
    build

    deploy
}

if [ "$#" -eq 0 ] ; then
    LOG "No arguments supplied. Defaulting to `all`"
    all
else
    if [ "$1" == "help" ] ; then
        cat <<EOF
Usage: ./build <task|help>
Tasks:
  - All -> Runs all tasks.
  - Deploy -> Deploys the program to a remote specified in settings.sh.
  - Build -> Builds the program.
  - Toolchain -> Grabs the Raspberry Pi toolchain (ignore on a raspberry pi).
  - Wiringpi -> Grabs and patches the WiringPi library.
  - Cleantoolchain -> Deletes the Raspberry pi toolchain.
  - Cleanwiringpi -> Deletes the WitingPi library.
  - Clean -> Deletes the build files.
  - Purge -> Deletes everything, the toolchain, WiringPi and build files.
EOF
    fi

    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "all" ] && all
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "toolchain" ] && toolchain
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "wiringpi" ] && wiringpi
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "cleantoolchain" ] && cleantoolchain
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "cleanwiringpi" ] && cleanwiringpi
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "clean" ] && clean
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "purge" ] && purge
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "deploy" ] && deploy
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "build" ] && build
fi

LOG "Nothing left to do. Done :^)"

