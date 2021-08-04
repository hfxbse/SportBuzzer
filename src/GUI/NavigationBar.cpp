//
// Created by stubit on 8/4/21.
//

#include "NavigationBar.hpp"

int16_t drawNavigationBar(Display &display, const Option *options, size_t optionCount) {
    display.setFont(Roboto_Medium8pt7b);

    int16_t upperBorder = 0;
    const float cellWidth = 100.0f / optionCount;

    for (int i = 0; i < optionCount; ++i) {
        uint16_t width, height;
        display.getTextBounds(options[i].text, &width, &height);

        upperBorder = !upperBorder ? Display::bottom(4) - height : upperBorder;
        const float verticalSpace = Display::bottom(0) - upperBorder - height;

        display.alignText(
                Display::left(((cellWidth / 2) + i * cellWidth) + 0.5),
                upperBorder + ((verticalSpace / 2) + 0.5) - 1,
                width,
                height,
                TextAlign::centered
        );

        const uint16_t fontColor = options[i].selected ? GxEPD_WHITE : GxEPD_BLACK;
        const uint16_t backgroundColor = options[i].selected ? GxEPD_BLACK : GxEPD_WHITE;

        display.drawRectangle(
                Display::left(cellWidth * i),
                upperBorder,
                Display::left(cellWidth * (i + 1)),
                Display::bottom(0),
                backgroundColor
        );

        display.setFontColor(fontColor);
        display.print(options[i].text);

        if (!options[i].selected && i != (optionCount - 1)) {
            const int16_t x = Display::right(cellWidth * (i + 1)) - 1;
            display.drawLine(x, upperBorder, x, Display::bottom(0));
        }
    }

    display.drawLine(Display::left(0), upperBorder, Display::right(0), upperBorder);
    display.drawLine(Display::left(0), Display::bottom(0), Display::right(0), Display::bottom(0));

    display.drawLine(Display::left(0), upperBorder, Display::left(0), Display::bottom(0));
    display.drawLine(Display::right(0), upperBorder, Display::right(0), Display::bottom(0));

    display.setFontColor(GxEPD_BLACK);

    return Display::bottom(0) - upperBorder;
}