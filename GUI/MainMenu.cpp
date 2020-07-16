//
// Created by stubit on 7/15/20.
//

#include <HardwareSerial.h>
#include "MainMenu.hpp"
#include "ChannelSelector.hpp"
#include "../Buttons.hpp"

GUITask * MainMenu::update(bool redraw) {
    GUITask * nextTask = this;

    if (redraw) {
        draw();
    } else {
        // region gui button handling
        Buttons::forEachButton([&](Button &button, Buttons::ButtonType type) {
            if (button.released()) {
                switch (type) {
                    case Buttons::ButtonType::previous:
                        previousMenuEntry();
                        draw();
                        break;
                        case Buttons::ButtonType::next:nextMenuEntry();
                        draw();
                        break;
                        case Buttons::ButtonType::confirm:for (auto &menuEntry : menuEntries) {
                            if (menuEntry.selected) {
                                nextTask = menuEntry.taskFactory();
                                Serial.println();
                                return;
                            }
                        }

                        Serial.println("ERROR: No menu entry selected.");
                        Serial.println();
                        break;
                }
            }
        });
        // endregion
    }

    return nextTask;
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
