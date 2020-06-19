cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "Arduino-CMake-Toolchain/Arduino-toolchain.cmake")

# setup project "SportBuzzer"
set(PROJECT_NAME SportBuzzer)
project(${PROJECT_NAME} CXX)

add_executable(
        ${PROJECT_NAME}
        sport_buzzer.cpp
        HC12.cpp HC12.hpp HC12_Options.hpp
        Transmissions.cpp Transmissions.hpp
        Button.cpp Button.hpp
)

target_compile_options(${PROJECT_NAME} PUBLIC -Os)

target_link_arduino_libraries(${PROJECT_NAME} PRIVATE SoftwareSerial core)

target_enable_arduino_upload(${PROJECT_NAME})
