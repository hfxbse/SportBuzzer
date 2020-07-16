//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "../Buttons.hpp"
#include "MainMenu.hpp"
#include "../Wireless/Communication.hpp"

GUITask * ChannelSelector::update(bool redraw) {
    GUITask * nextTask = this;

    if (redraw) {
        draw();
    } else {
        Buttons::forEachButton([&](Button &button, Buttons::ButtonType type) {
            if (button.released()) {
                if (!selecting && type != Buttons::ButtonType::confirm) {
                    // navigation
                    onSelector = !onSelector;
                    draw();
                } else if (type == Buttons::ButtonType::confirm) {
                    if (!onSelector) {
                        // exit channel selector, apply channel
                        if (previousChannel != channel) {
                            Serial.println("Applying Channel");

                            Communication::checkHC12Result(
                                    Communication::hc12.setChannel(channel),
                                    "Could not apply channel \n");
                        }

                        nextTask = new MainMenu();
                        return;
                    } else {
                        // change between navigation and channel selection
                        selecting = !selecting;
                        draw();
                    }
                } else {
                    // region change channel
                    if (type == Buttons::ButtonType::previous && channel != 1) {
                        channel = static_cast<Channel>(channel - 1);
                        draw();
                    } else if (type == Buttons::ButtonType::next && channel != 100) {
                        channel = static_cast<Channel>(channel + 1);
                        draw();
                    }
                }
            }
        });
    }

    return nextTask;
}

void ChannelSelector::draw() {
    Serial.println("Channel selector");
    Serial.println("Buzzers need to be in the same channel to be able to communicate.");

    if (selecting) {
        Serial.print("+");
    } else if (onSelector) {
        Serial.print("x");
    } else {
        Serial.print(" ");
    }

    Serial.print("  Current channel: ");
    Serial.println(channel);

    if (!onSelector) {
        Serial.print("x");
    } else {
        Serial.print(" ");
    }

    Serial.println("  OK");

    Serial.println();
}
