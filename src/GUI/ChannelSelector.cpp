//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"
#include <src/Wireless/Connection.hpp>
#include "fonts/RobotoMono_Regular11pt7b.h"
#include "fonts/Roboto_Thin7pt7b.h"
#include "NavigationBar.hpp"
#include "LabeledString.hpp"
#include "NumberInput.hpp"

GUITask *ChannelSelector::update(Display &display, Transmissions &, unsigned long, bool redraw, uint16_t yOffset) {
    if (redraw) {
        draw(display, yOffset);
    } else {
        GUIInput input;
        input.poll();

        if (!selecting && (input.next() || input.previous())) {
            // navigation
            onSelector = !onSelector;
            draw(display, yOffset);
            display.update();
        } else if (input.confirm() && !selecting) {
            if (!onSelector) {
                return new MainMenu();
            } else {
                // change between navigation and channel selection
                selecting = true;
                digitOffset = 0;
                draw(display, yOffset);
                display.update();
            }
        } else if (selecting) {
            int stepSize = pow(10, 2 - digitOffset);

            // region change channel
            if (input.previous() && (selectedChannel / stepSize) % 10 != 0) {
                selectedChannel = static_cast<Channel>(selectedChannel - stepSize);
                draw(display, yOffset);
                display.update();
            } else if (input.next() && (selectedChannel / stepSize) % 10 != (digitOffset != 0 ? 9 : 1)) {
                selectedChannel = static_cast<Channel>(selectedChannel + stepSize);
                draw(display, yOffset);
                display.update();
            } else if (input.confirm()) {
                // region change digit offset
                ++digitOffset;

                if (digitOffset > 2) {
                    selecting = false;

                    if (selectedChannel > 100) {
                        selectedChannel = static_cast<Channel>(100);
                    } else if (!selectedChannel) {
                        selectedChannel = static_cast<Channel>(1);
                    }
                }
                // endregion

                draw(display, yOffset);

                // region apply channel
                //
                // changing channel triggers update in gui task handling
                // therefore only force the display to update when the channel didn't change
                //

                if (!selecting) {
                    if (selectedChannel != channel) {
                        Serial.println("Applying Channel");

                        bool success = Connection::hc12.setChannel(selectedChannel);
                        if (success) {
                            channel = selectedChannel;
                            Serial.println();
                        } else {
                            Serial.println("ERROR: Could not apply Channel.\n");
                        }
                    } else {
                        display.update();
                    }
                } else {
                    display.update();
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

void ChannelSelector::draw(Display &display, uint16_t yOffset) {
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

    // region draw channel
    const uint16_t navigationHeight = drawNavigationBar(display, options, 2);
    const uint16_t availableHeight = getAvailableHeight(navigationHeight + yOffset);

    uint16_t width, height;

    display.setFont(RobotoMono_Regular11pt7b);
    display.getTextBounds("000", &width, &height);

    const uint16_t yAlign = alignVertically(yOffset, availableHeight, height) - getMargin(5, availableHeight);

    display.alignText(
            Display::right(36),
            yAlign,
            width,
            height,
            TextAlign::centered
    );
    display.print(currentChannel(selectedChannel));

    if (selecting) {
        drawCursor(
                display,
                "000",
                currentChannel(selectedChannel),
                digitOffset,
                Display::right(36) + ((static_cast<float>(width) / 2) + 0.5),
                height,
                yAlign,
                availableHeight
        );
    }

    display.setFont(Roboto_Thin7pt7b);
    display.getTextBounds("Funkkanal", &width, &height);

    display.alignText(
            Display::left(36),
            alignVertically(yOffset, availableHeight, height) - getMargin(5, availableHeight),
            width,
            height,
            TextAlign::centered
    );

    display.print("Funkkanal");
    // endregion

    // region draw border
    display.drawLine(
            Display::left(0),
            Display::top(0) + yOffset,
            Display::left(0),
            Display::bottom(0) - navigationHeight
    );

    display.drawLine(
            Display::right(0),
            Display::top(0) + yOffset,
            Display::right(0),
            Display::bottom(0) - navigationHeight
    );
    // endregion

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
