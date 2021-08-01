cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "Arduino-CMake-Toolchain/Arduino-toolchain.cmake")

# setup project "SportBuzzer"
set(PROJECT_NAME SportBuzzer)
project(${PROJECT_NAME} CXX)

file(GLOB_RECURSE SOURCES "src/*.*")

file(GLOB LIBRARIES "libraries/*")

include_directories(${LIBRARIES} "libraries/GxEPD2/src/")

add_executable(
        ${PROJECT_NAME}
        SportBuzzer.cpp
        ${SOURCES}
)

target_compile_options(${PROJECT_NAME} PUBLIC -Os)

target_link_arduino_libraries(${PROJECT_NAME} AUTO_PRIVATE)
target_link_arduino_libraries(${PROJECT_NAME} PRIVATE GxEPD2)

target_enable_arduino_upload(${PROJECT_NAME})
