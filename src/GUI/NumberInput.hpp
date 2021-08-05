//
// Created by stubit on 8/5/21.
//

#ifndef SPORTBUZZER_NUMBERINPUT_HPP
#define SPORTBUZZER_NUMBERINPUT_HPP

#include "../HardwareInterfaces/Display.hpp"

// TODO should be refactored to need less parameter
void drawCursor(
        Display &display,
        const String &maxString,
        const String &fullString,
        byte position,
        uint16_t xEnd,
        uint16_t height,
        uint16_t yOffset,
        uint16_t availableHeight
);

#endif //SPORTBUZZER_NUMBERINPUT_HPP
