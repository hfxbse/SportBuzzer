//
// Created by stubit on 7/15/20.
//

#include <HardwareSerial.h>
#include "MainMenu.hpp"
#include "ChannelSelector.hpp"
#include "GUIInput.hpp"

GUITask *MainMenu::update(Transmissions &, unsigned long, bool redraw) {
    if (redraw) {
        draw();
    } else {
        GUIInput input;
        input.poll();

        if (input.previous()) {
            previousMenuEntry();
            draw();
        } else if (input.next()) {
            nextMenuEntry();
            draw();
        } else if (input.confirm()) {
            for (auto &menuEntry : menuEntries) {
                if (menuEntry.selected) {
                    Serial.println();
                    return menuEntry.taskFactory();
                }
            }

            Serial.println("ERROR: No menu entry selected.");
            Serial.println();
        }
        // endregion
    }

    return this;
}

void MainMenu::draw() {
    Serial.println("Menu");

    for (const auto &menuEntry:menuEntries) {
        if (menuEntry.selected) {
            Serial.print("x  ");
        } else {
            Serial.print("   ");
        }

        Serial.println(menuEntry.text);
    }

    Serial.println();
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
