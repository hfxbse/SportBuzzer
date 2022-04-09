//
// Created by stubit on 7/15/20.
//

#include <HardwareSerial.h>
#include "MainMenu.hpp"
#include "ChannelSelector.hpp"
#include "GUIInput.hpp"
#include "src/GUI/fonts/Roboto_Medium9pt7b.h"

GUITask *MainMenu::update(Display &display, Transmissions &, unsigned long, bool redraw, uint16_t yOffset) {
    if (redraw) {
        draw(display, yOffset);
    } else {
        GUIInput input;
        input.poll();

        if (input.previous()) {
            previousMenuEntry();
            draw(display, yOffset);
            display.update();
        } else if (input.next()) {
            nextMenuEntry();
            draw(display, yOffset);
            display.update();
        } else if (input.confirm()) {
            for (auto &menuEntry : menuEntries) {
                if (menuEntry.selected) {
                    return menuEntry.taskFactory();
                }
            }
        }
        // endregion
    }

    return this;
}

void MainMenu::draw(Display &display, uint16_t yOffset) {
    display.drawLine(Display::left(0), yOffset + Display::top(0), Display::left(0), Display::bottom(0));
    display.drawLine(Display::right(0), yOffset + Display::top(0), Display::right(0), Display::bottom(0));

    const uint16_t entryHeight = (float(100) / MENU_LENGTH) + 0.5;
    const uint16_t menuHeight = DISPLAY_HEIGHT - yOffset;

    // region draw every menu entry
    for (int i = 0; i < MENU_LENGTH; ++i) {
        const uint16_t lineOffset = Display::top(entryHeight * (i + 1) + i, menuHeight) + yOffset;

        if (menuEntries[i].selected) {
            const uint16_t rowStart = Display::top(entryHeight * i + i, menuHeight) + yOffset;

            display.drawRectangle(Display::left(0), rowStart, Display::right(0), lineOffset);
            display.setFontColor(GxEPD_WHITE);

        } else {
            display.setFontColor(GxEPD_BLACK);

            if (i != MENU_LENGTH - 1) {
                display.drawLine(Display::left(0), lineOffset, Display::right(0), lineOffset);
            } else {
                display.drawLine(Display::left(0), Display::bottom(0), Display::right(0), Display::bottom(0));
            }
        }

        display.setFont(Roboto_Medium9pt7b);

        uint16_t width, height;
        display.getTextBounds(menuEntries[i].text, &width, &height);
        display.alignText(
                Display::left(50),
                lineOffset - entryHeight + ((entryHeight - height) / 2) - 1,
                width,
                height,
                TextAlign::centered
        );

        display.print(menuEntries[i].text);
    }
    // endregion
}

void MainMenu::previousMenuEntry() {
    for (int i = 0; i < MENU_LENGTH; ++i) {
        if (menuEntries[i].selected) {
            menuEntries[i].selected = false;

            if (i == 0) {
                menuEntries[MENU_LENGTH - 1].selected = true;
            } else {
                menuEntries[i - 1].selected = true;
            }

            return;
        }
    }
}

void MainMenu::nextMenuEntry() {
    for (int i = 0; i < MENU_LENGTH; ++i) {
        if (menuEntries[i].selected) {
            menuEntries[i].selected = false;

            if (i + 1 == MENU_LENGTH) {
                menuEntries[0].selected = true;
            } else {
                menuEntries[i + 1].selected = true;
            }

            return;
        }
    }
}
