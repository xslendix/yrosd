#!/usr/bin/bash

LOG() {
    echo -e "\e[31m :: \e[0m$@"
}

if ! command -v cmake &> /dev/null; then
    LOG "CMake could not be found. Please install it."
    exit
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

set -e

toolchain() {
    if [ $TOOLCHAIN_ENABLED -ne 0 ] ; then
        if [ ! -f "toolchain-rpi.cmake" ] ; then
            LOG "Creating toolchain file"
            sed "s|__TOOLPATH__|$PWD/tools|g" toolchain-rpi.template.cmake > toolchain-rpi.cmake
        fi

        if [ ! -d "$PWD/tools" ] ; then
            LOG "Getting toolchain"
            git clone --depth 1 https://github.com/rvagg/rpi-newer-crosstools tools
        fi
    fi
}

wiringpi () {
    if [ ! -f "vendor/WiringPi/.patched" ] ; then
        LOG "Getting WiringPi"
        git clone --depth 1 https://github.com/WiringPi/WiringPi vendor/WiringPi

        LOG "Patching WiringPi"
        # sed -i '782,822d' $PWD/vendor/WiringPi/wiringPi/wpiExtensions.c
        # sed -i '531,536d' $PWD/vendor/WiringPi/wiringPi/wpiExtensions.c
        # sed -i '64d' $PWD/vendor/WiringPi/wiringPi/wpiExtensions.c
        # sed -i '58d' $PWD/vendor/WiringPi/wiringPi/wpiExtensions.c
        # rm -f $PWD/vendor/WiringPi/wiringPi/drcNet.{h,c}
        # rm -f $PWD/vendor/WiringPi/wiringPi/rht03.{h,c}

        # for file in $PWD/vendor/WiringPi/wiringPi/*c; do
        #     sed -i '1s/^/#include <time.h>\n/' $file
        # done

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

opencv() {
    toolchain

    OPENCV_VERSION=4.5.4-2
    if [ $TOOLCHAIN_ENABLED -ne 0 ] ; then
        if [ ! -f "tools/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/arm-rpi-linux-gnueabihf/sysroot/usr/local/share/opencv4/haarcascades/haarcascade_eye.xml" ] ; then
            LOG "Getting OpenCV"
            cd tools/x64-gcc-8.3.0/arm-rpi-linux-gnueabihf/arm-rpi-linux-gnueabihf/sysroot
            wget https://github.com/prepkg/opencv-raspberrypi/releases/download/$OPENCV_VERSION/opencv.deb
            ar x opencv.deb
            tar xvf data.tar.xz
            rm -f control.tar.xz data.tar.xz dedian-binary opencv.*
        fi
    fi

    # OPENCV_VERSION=3.4.0
    # if [ ! -f "vendor/opencv-$OPENCV_VERSION/.patched" ] ; then
    #     LOG "Getting OpenCV"
    #     [ ! -f "/tmp/opencv.zip" ] && wget -O /tmp/opencv.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip
    #     [ ! -f "/tmp/opencv-contrib.zip" ] && wget -O /tmp/opencv-contrib.zip https://github.com/opencv/opencv_contrib/archive/$OPENCV_VERSION.zip
    #     cd vendor
    #     rm -rf opencv*
    #     unzip /tmp/opencv.zip
    #     # unzip /tmp/opencv-contrib.zip

    #     LOG "Patching OpenCV"
    #     # sed -i '259d' $PWD/opencv-$OPENCV_VERSION/cmake/OpenCVCompilerOptions.cmake

    #     LOG "Building OpenCV"

    #     cd opencv-$OPENCV_VERSION
    #     mkdir -p build
    #     cd build

    #     cmake \
    #         -D CV_DISABLE_OPTIMIZATION=ON -D CPU_BASELINE="" -D CPU_DISPATCH="" \
    #         -D CMAKE_BUILD_TYPE=RELEASE \
    #         -D CMAKE_INSTALL_PREFIX=$PWD/../../opencv \
    #         -D INSTALL_PYTHON_EXAMPLES=OFF \
    #         -D BUILD_EXAMPLES=OFF .. \
    #         -D WITH_GTK=OFF \
    #         -D WITH_GTK_2_X=OFF \
    #         -DCMAKE_TOOLCHAIN_FILE=../../../toolchain-rpi.cmake \
    #         -G Ninja

    #         # -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-$OPENCV_VERSION/modules \
    #         # -D WITH_CUDA=OFF \
    #     
    #     ninja
    #     ninja install

    #     LOG "Compressing OpenCV"
    #     tar -zcvf ../../opencv_build.tar.gz install

    #     cd ../..

    #     touch vendor/opencv-$OPENCV_VERSION/.patched
    #fi
}

cleantoolchain() {
    LOG "Deleting toolchain"
    rm -rf tools toolchain-rpi.cmake
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
    scp -P$RPI_PORT build/dripd $RPI_USER@$RPI_HOST:/usr/local/bin
    ssh -p$RPI_PORT $SPI_USER@$RPI_HOST "chmod +x /usr/local/bin/dripd"
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
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "opencv" ] && opencv
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "cleantoolchain" ] && cleantoolchain
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "cleanwiringpi" ] && cleanwiringpi
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "clean" ] && clean
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "purge" ] && purge
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "deploy" ] && deploy
    [ "$(echo $1 | tr '[:upper:]' '[:lower:]')" == "build" ] && build
fi

LOG "Nothing left to do. Done :^)"

