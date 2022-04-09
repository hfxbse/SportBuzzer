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
    const uint16_t barHeight = lowerBound - Display::top(0);

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
    const uint16_t leftOffset = Display::right(15);
    const uint16_t rightOffset = Display::right(1) - 3;
    const uint16_t topOffset = Display::top(15, barHeight) + 1;
    const uint16_t bottomOffset = Display::bottom(15, barHeight);
    const uint16_t batteryTopTopOffset = Display::top(25, barHeight) + 1;
    const uint16_t batteryTopBottomOffset = Display::bottom(25, barHeight);

    display.drawRectangle(rightOffset + 1, batteryTopTopOffset, rightOffset + 2, batteryTopBottomOffset);
    display.drawRectangle(leftOffset, topOffset, rightOffset, bottomOffset);
    display.drawRectangle(leftOffset + 1, topOffset + 1, rightOffset - 1, bottomOffset - 1, GxEPD_WHITE);

    display.drawLine(
            rightOffset + 1,
            batteryTopTopOffset + 1,
            rightOffset + 1,
            batteryTopBottomOffset - 1,
            GxEPD_WHITE
    );

    display.drawRectangle(
            leftOffset + 2,
            topOffset + 2,
            leftOffset +
            (constrain(batteryLevel / 5 + 1, 1, 20) * static_cast<float>(rightOffset - leftOffset - 2) / 20.0f),
            bottomOffset - 2
    );

    // ensure that there is at least one bar battery visible if it's able to power up
    display.drawLine(leftOffset + 2, topOffset + 2, leftOffset + 2, bottomOffset - 2);

    // endregion

    return barHeight;
}

#endif //SPORTBUZZER_STATUSBAR_HPP
