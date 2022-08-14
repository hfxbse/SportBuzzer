//
// Created by stubit on 8/5/21.
//

#include <src/HardwareInterfaces/Display.hpp>
#include "LabeledString.hpp"

String digits(uint16_t number, size_t min) {
    String string = String(number);

    while (string.length() < min) {
        string = "0" + string;
    }

    return string;
}

uint16_t getAvailableHeight(uint16_t offsets) {
    return Display::bottom(0) - Display::top(0) - offsets;
}

uint16_t getMargin(float percentage, uint16_t availableHeight) {
    return Display::top(percentage, availableHeight) - Display::top(0);
}

uint16_t alignVertically(uint16_t yOffset, uint16_t availableHeight, uint16_t requiredSpace) {
    return Display::top(0) + yOffset + (
            static_cast<float>(availableHeight - requiredSpace) / 2 - 0.5
    );
}

String getTimeString(unsigned long duration, bool hidden) {
    if (hidden) {
        return "--:--:--:--:---";
    }

    const uint16_t days = duration / (DAY);
    duration -= DAY * days;

    const uint16_t hours = duration / HOUR;
    duration -= HOUR * hours;

    const uint16_t minutes = duration / MINUTE;
    duration -= MINUTE * minutes;

    const uint16_t seconds = duration / SECOND;
    duration -= SECOND * seconds;

    return digits(days) + ":" +
           digits(hours) + ":" +
           digits(minutes) + ":" +
           digits(seconds) + ":" +
           digits(duration, 3);
}
