cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_TOOLCHAIN_FILE "Arduino-CMake-Toolchain/Arduino-toolchain.cmake")
set(ARDUINO_BOARD_OPTIONS_FILE "cmake-build-debug/BoardOptions.cmake")

set(PROJECT_NAME SportBuzzer)

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} sport_buzzer.cpp)

# Arduino.h is included in hello_world.cpp, so link with Arduino core
target_link_arduino_libraries(${PROJECT_NAME} PRIVATE core)

# This is needed for the generation of HEX binary and uploading it
target_enable_arduino_upload(${PROJECT_NAME})
