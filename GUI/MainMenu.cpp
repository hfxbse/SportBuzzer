//
// Created by stubit on 7/15/20.
//

#include <HardwareSerial.h>
#include "MainMenu.hpp"
#include "ChannelSelector.hpp"
#include "../Buttons.hpp"

namespace MainMenu {
    void drawMenu(const MenuEntry menuEntries[], size_t size) {
        Serial.println("Menu");

        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                Serial.print("x  ");
            } else {
                Serial.print("   ");
            }

            Serial.println(menuEntries[i].text);
        }

        Serial.println();
    }

    void previousMenuEntry(MenuEntry *menuEntries, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                menuEntries[i].selected = false;

                if (i == 0) {
                    menuEntries[size - 1].selected = true;
                } else {
                    menuEntries[i - 1].selected = true;
                }

                return;
            }
        }
    }

    void nextMenuEntry(MenuEntry *menuEntries, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                menuEntries[i].selected = false;

                if (i + 1 == size) {
                    menuEntries[0].selected = true;
                } else {
                    menuEntries[i + 1].selected = true;
                }

                return;
            }
        }
    }

    GUITask mainMenu(bool redraw) {
        const size_t menuLength = 3;
        static MenuEntry menuEntries[] = {
                MenuEntry("Entry 1", nullptr, true),
                MenuEntry("Entry 2", nullptr),
                MenuEntry("Channel selection", reinterpret_cast<GUITask>(ChannelSelector::channelSelector)),
        };

        auto nextTask = reinterpret_cast<GUITask>(mainMenu);

        if (redraw) {
            // region reset selection
            for (int i = 1; i < menuLength; ++i) {
                menuEntries[i].selected = false;
            }

            menuEntries[0].selected = true;
            // endregion

            drawMenu(menuEntries, menuLength);
        } else {
            // region gui button handling
            Buttons::forEachButton([&](Button &button, Buttons::ButtonType type) {
                if (button.released()) {
                    switch (type) {
                        case Buttons::ButtonType::previous:
                            previousMenuEntry(menuEntries, menuLength);
                            drawMenu(menuEntries, menuLength);
                            break;

                        case Buttons::ButtonType::next:
                            nextMenuEntry(menuEntries, menuLength);
                            drawMenu(menuEntries, menuLength);
                            break;

                        case Buttons::ButtonType::confirm:
                            for (auto &menuEntry : menuEntries) {
                                if (menuEntry.selected) {
                                    nextTask = menuEntry.task;
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
}