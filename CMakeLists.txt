# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++")

set(EXPORT_COMPILE_COMMANDS ON)

option(USE_GUI "Use GUI" OFF)
# Set the project name
project(yrosd)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/Common.cpp
    src/Config.cpp
    src/Application.cpp
    src/CommandParser.cpp
    src/Messaging.cpp
    src/Backend/SimpleBackend.cpp
    src/Backend/PigpioBackend.cpp
    src/Drivers/MotorDriver.cpp
    src/Servers/Server.cpp
    src/Servers/MainServer.cpp
    src/Servers/VideoServer.cpp
    src/Signals.cpp
    src/Processes.cpp
    src/main.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Add an executable with the above sources
add_executable(yrosd ${SOURCES})

install(TARGETS yrosd DESTINATION bin)

find_package(Threads REQUIRED)

find_package(OpenCV COMPONENTS core imgcodecs videoio)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(pigpio)
if(${PIGPIO_FOUND})
    include_directories(${pigpio_INCLUDE_DIRS})
    set(LIBS ${pigpio_LIBS} ${pigpio_LIBRARY})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPREFERRED_BACKEND=2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPREFERRED_BACKEND=1")
endif()

find_package(spdlog REQUIRED)

set(LIBS Threads::Threads ${OpenCV_LIBS} spdlog::spdlog ${LIBS})

if(USE_GUI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_GUI")
endif(USE_GUI)

target_link_libraries(yrosd PRIVATE ${LIBS})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(yrosd
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/vendor/cpp-mjpeg-streamer/include
    ${PROJECT_SOURCE_DIR}/vendor/argparse/include
    ${PROJECT_SOURCE_DIR}/vendor/spdlog/include
    ${PROJECT_SOURCE_DIR}/vendor/tomlplusplus/include
)

