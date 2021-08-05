//
// Created by stubit on 8/5/21.
//

#include "NumberInput.hpp"
#include <src/HardwareInterfaces/Display.hpp>
#include "LabeledString.hpp"

void drawCursor(
        Display &display,
        const String &maxString,
        const String &fullString,
        byte position,
        uint16_t xEnd,
        uint16_t height,
        uint16_t yOffset,
        uint16_t availableHeight
) {
    uint16_t displacement, width;
    display.getTextBounds(maxString.substring(position), &displacement, nullptr);
    display.getTextBounds("0", &width, nullptr);

    display.alignText(xEnd - displacement, yOffset, width, height);
    display.setFontColor(GxEPD_WHITE);

    display.drawRectangle(
            xEnd - displacement,
            yOffset - getMargin(2, availableHeight),
            xEnd - displacement + width + 1,
            yOffset + height + getMargin(2, availableHeight)
    );

    display.print(String(fullString[position]));

    display.setFontColor(GxEPD_BLACK);
}
