//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"
#include "../Wireless/Connection.hpp"

GUITask *ChannelSelector::update(Transmissions &, unsigned long, bool redraw) {
    if (redraw) {
        draw();
    } else {
        GUIInput input;
        input.poll();

        if (!selecting && (input.next() || input.previous())) {
            // navigation
            onSelector = !onSelector;
            draw();
        } else if (input.confirm() && !selecting) {
            if (!onSelector) {
                return new MainMenu();
            } else if (onSelector) {
                // change between navigation and channel selection
                selecting = true;
                digitOffset = 0;
                draw();
            }
        } else if (selecting) {
            int stepSize = pow(10, digitOffset) + (digitOffset == 2);

            // region change channel
            if (input.previous() && (channel / stepSize) % 10 != 0) {
                channel = static_cast<Channel>(channel - stepSize);
                draw();
            } else if (input.next() && (channel / stepSize) % 10 != (digitOffset != 2 ? 9 : 1)) {
                channel = static_cast<Channel>(channel + stepSize);
                draw();
            } else if (input.confirm()) {
                // region change digit offset
                ++digitOffset;

                if (digitOffset > 2) {
                    selecting = false;

                    if (channel > 100) {
                        channel = static_cast<Channel>(100);
                    } else if (!channel) {
                        channel = static_cast<Channel>(1);
                    }
                }

                draw();
                // endregion

                // region apply channel
                if (!selecting) {
                    if (previousChannel != channel) {
                        Serial.println("Applying Channel");

                        bool success = Connection::hc12.setChannel(channel);
                        if (success) {
                            previousChannel = channel;
                            Serial.println();
                        } else {
                            Serial.println("ERROR: Could not apply Channel.\n");
                        }
                    }
                }
                // endregion
            }
        }
    }

    return this;
}

void ChannelSelector::draw() {
    Serial.println("Channel selector");
    Serial.println("Buzzers need to be in the same channel to be able to communicate.");

    if (selecting) {
        Serial.print(digitOffset + 1);
    } else if (onSelector) {
        Serial.print("x");
    } else {
        Serial.print(" ");
    }

    String channelString(channel);

    while (channelString.length() < 3) {
        channelString = "0" + channelString;
    }

    Serial.print("  Current channel: ");
    Serial.println(channelString);

    if (!onSelector) {
        Serial.print("x");
    } else {
        Serial.print(" ");
    }

    Serial.println("  OK");

    Serial.println();
}
