cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "Arduino-CMake-Toolchain/Arduino-toolchain.cmake")

# setup project "SportBuzzer"
set(PROJECT_NAME SportBuzzer)
project(${PROJECT_NAME} CXX)

add_executable(
        ${PROJECT_NAME}
        SportBuzzer.cpp
        src/HardwareInterfaces/HC12.cpp src/HardwareInterfaces/HC12.hpp src/HardwareInterfaces/HC12_Options.hpp
        src/Wireless/Transmissions.cpp src/Wireless/Transmissions.hpp
        src/HardwareInterfaces/Button.cpp src/HardwareInterfaces/Button.hpp
        src/GUI/MainMenu.cpp src/GUI/MainMenu.hpp
        src/GUI/GUIInput.hpp src/GUI/GUIInput.cpp
        src/GUI/ChannelSelector.cpp src/GUI/ChannelSelector.hpp
        src/GUI/GUITask.hpp
        src/Wireless/Communication.cpp src/Wireless/Communication.hpp
        src/GUI/MenuEntry.cpp src/GUI/MenuEntry.hpp
        src/GUI/Stopwatch.cpp src/GUI/Stopwatch.hpp
)

target_compile_options(${PROJECT_NAME} PUBLIC -Os)

target_link_arduino_libraries(${PROJECT_NAME} PRIVATE SoftwareSerial core)

target_enable_arduino_upload(${PROJECT_NAME})
