# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++")

# Set the project name
project(yrosd)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/Common.cpp
    src/Config.cpp
    src/Application.cpp
    src/CommandParser.cpp
    src/Servers/Server.cpp
    src/Servers/MainServer.cpp
    src/Servers/UDPServer.cpp
    src/Servers/VideoServer.cpp
    src/Signals.cpp
    src/Processes.cpp
    src/main.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add an executable with the above sources
add_executable(yrosd ${SOURCES})

find_package(Threads REQUIRED)
target_link_libraries(yrosd PRIVATE Threads::Threads)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(wiringPi)
include_directories(${wiringPi_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

target_link_libraries(yrosd PRIVATE ${OpenCV_LIBS} ${pigpio_LIBS} ${wiringPi_LIBS} spdlog::spdlog)

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

install(TARGETS yrosd DESTINATION bin)

