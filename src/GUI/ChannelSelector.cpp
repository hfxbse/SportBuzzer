//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"
#include "../Wireless/Communication.hpp"

GUITask *ChannelSelector::update(bool redraw) {
    if (redraw) {
        draw();
    } else {
        GUIInput input;
        input.poll();

        if (!selecting && (input.next() || input.previous())) {
            // navigation
            onSelector = !onSelector;
            draw();
        } else if (input.confirm()) {
            if (!onSelector) {
                // exit channel selector, apply channel
                if (previousChannel != channel) {
                    Serial.println("Applying Channel");

                    Communication::checkHC12Result(
                            Communication::hc12.setChannel(channel),
                            "Could not apply channel \n");
                }

                return new MainMenu();
            } else {
                // change between navigation and channel selection
                selecting = !selecting;
                draw();
            }
        } else if (selecting) {
            // region change channel
            if (input.previous() && channel != 1) {
                channel = static_cast<Channel>(channel - 1);
                draw();
            } else if (input.next() && channel != 100) {
                channel = static_cast<Channel>(channel + 1);
                draw();
            }
        }
    }

    return this;
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
