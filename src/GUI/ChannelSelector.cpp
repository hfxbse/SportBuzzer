//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"
#include <src/Wireless/Connection.hpp>
#include "NavigationBar.hpp"

GUITask *ChannelSelector::update(Display &display, Transmissions &, unsigned long, bool redraw, uint16_t yOffset) {
    if (redraw) {
        draw(display);
    } else {
        GUIInput input;
        input.poll();

        if (!selecting && (input.next() || input.previous())) {
            // navigation
            onSelector = !onSelector;
            draw(display);
            display.update();
        } else if (input.confirm() && !selecting) {
            if (!onSelector) {
                return new MainMenu();
            } else {
                // change between navigation and channel selection
                selecting = true;
                digitOffset = 0;
                draw(display);
                display.update();
            }
        } else if (selecting) {
            int stepSize = pow(10, digitOffset) + (digitOffset == 2);

            // region change channel
            if (input.previous() && (channel / stepSize) % 10 != 0) {
                channel = static_cast<Channel>(channel - stepSize);
                draw(display);
            } else if (input.next() && (channel / stepSize) % 10 != (digitOffset != 2 ? 9 : 1)) {
                channel = static_cast<Channel>(channel + stepSize);
                draw(display);
                display.update();
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

                draw(display);
                display.update();
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

String ChannelSelector::currentChannel(Channel channel) {
    String channelString(channel);

    while (channelString.length() < 3) {
        channelString = "0" + channelString;
    }

    return channelString;
}

void ChannelSelector::draw(Display &display) {
    Serial.println("Channel selector");
    Serial.println("Buzzers need to be in the same channel to be able to communicate.");

    if (selecting) {
        // print current position of digit which is getting modified, if the channel number is getting modified
        Serial.print(digitOffset + 1);
    } else if (onSelector) {
        // show cursor on channel number field, but it is not getting modified
        Serial.print("x");
    } else {
        // channel number is not getting modified and the cursor is not on the input field
        Serial.print(" ");
    }

    Option options[2] = {
            Option("Verlassen", !onSelector),
            Option("Wechseln", onSelector && !selecting)
    };

    drawNavigationBar(display, options, 2);

    // region print current channel, always with 3 displayed digits
    String channelString = currentChannel();

    Serial.print("  Current channel: ");
    Serial.println(channelString);
    // endregion

    // region draw confirm button
    if (!onSelector) {      // cursor is on the button
        Serial.print("x");
    } else {    // cursor is on not the button
        Serial.print(" ");
    }

    Serial.println("  OK");
    // region

    Serial.println();
}
