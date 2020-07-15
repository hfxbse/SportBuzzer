cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_TOOLCHAIN_FILE "Arduino-CMake-Toolchain/Arduino-toolchain.cmake")

# setup project "SportBuzzer"
set(PROJECT_NAME SportBuzzer)
project(${PROJECT_NAME} CXX)

add_executable(
        ${PROJECT_NAME}
        SportBuzzer.cpp
        HardwareInterfaces/HC12.cpp HardwareInterfaces/HC12.hpp HardwareInterfaces/HC12_Options.hpp
        Wireless/Transmissions.cpp Wireless/Transmissions.hpp
        HardwareInterfaces/Button.cpp HardwareInterfaces/Button.hpp
        GUI/MainMenu.cpp GUI/MainMenu.hpp
        Buttons.hpp Buttons.cpp
        GUI/ChannelSelector.cpp GUI/ChannelSelector.hpp
        GUI/GUITask.hpp
        Wireless/Communication.cpp Wireless/Communication.hpp
)

target_compile_options(${PROJECT_NAME} PUBLIC -Os)

target_link_arduino_libraries(${PROJECT_NAME} PRIVATE SoftwareSerial core)

target_enable_arduino_upload(${PROJECT_NAME})
