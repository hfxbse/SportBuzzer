//
// Created by stubit on 8/3/21.
//

#ifndef SPORTBUZZER_STATUSBAR_HPP
#define SPORTBUZZER_STATUSBAR_HPP

#include "src/HardwareInterfaces/Display.hpp"
#include "src/GUI/fonts/Roboto_Thin7pt7b.h"
#include "ChannelSelector.hpp"

uint16_t drawStatusBar(Display &display, bool connected, int batteryLevel) {
    display.setFont(Roboto_Thin7pt7b);

    uint16_t maxWidth, width, height;

    // region draw channel
    display.getTextBounds("100", &maxWidth, nullptr);
    display.getTextBounds(ChannelSelector::currentChannel(), &width, &height);

    // region prepare area
    const uint16_t lowerBound = Display::top(3 + height + 1);
    display.drawRectangle(Display::left(0), Display::top(0), Display::right(0), lowerBound, GxEPD_WHITE);

    display.drawLine(Display::left(0), lowerBound, Display::right(0), lowerBound);
    display.drawLine(Display::left(0), Display::top(0), Display::right(0), Display::top(0));

    display.drawLine(Display::left(0), Display::top(0), Display::left(0), lowerBound);
    display.drawLine(Display::right(0), Display::top(0), Display::right(0), lowerBound);
    // endregion draw bar border

    const uint16_t upperTextBound = Display::top(2) + 1;

    display.alignText(Display::left(1) + maxWidth, upperTextBound, width, height, TextAlign::right);
    display.print(ChannelSelector::currentChannel());
    // endregion draw channel

    // region draw separator between channel and connection status
    const uint16_t separator = Display::left(1.25) + maxWidth + Display::left(2);

    display.drawLine(separator, upperTextBound, separator, upperTextBound + height + 1);
    // endregion draw separator between channel and connection status

    // region draw connection status
    const String status = connected ? "Verbunden" : "Nicht verbunden";

    display.getTextBounds(status, &width, &height);
    display.alignText(separator + Display::left(2), upperTextBound - 1, width, height);

    display.print(status);
    // endregion draw connection status

    // region draw battery level
    const String level = String(batteryLevel) + "%";

    display.getTextBounds("100%", &maxWidth, nullptr);
    display.getTextBounds(level, &width, &height);

    const uint16_t rightOffset = Display::right(2) - 1;

    display.alignText(rightOffset, upperTextBound, width, height, TextAlign::right);
    display.print(level);

    display.getTextBounds("Akku", &width, &height);
    display.alignText(rightOffset - maxWidth - Display::left(2), upperTextBound - 1, width, height, TextAlign::right);
    display.print("Akku");
    // endregion

    return lowerBound - Display::top(0);
}

#endif //SPORTBUZZER_STATUSBAR_HPP
