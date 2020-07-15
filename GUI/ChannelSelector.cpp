//
// Created by stubit on 7/15/20.
//

#include "ChannelSelector.hpp"
#include "../Buttons.hpp"
#include "MainMenu.hpp"
#include "../Wireless/Communication.hpp"

namespace ChannelSelector {
    GUITask channelSelector(bool redraw) {
        static Channel channel = C001;
        static Channel prevChannel = C001;
        static bool onSelector = true, selecting = false;

        auto nextTask = reinterpret_cast<GUITask>(channelSelector);

        if (redraw) {
            onSelector = true;
            selecting = false;
            drawChannelSelector(channel, onSelector, selecting);
        } else {
            Buttons::forEachButton([&](Button &button, Buttons::ButtonType type) {
                if (button.released()) {
                    if (!selecting && type != Buttons::ButtonType::confirm) {
                        // navigation
                        onSelector = !onSelector;
                        drawChannelSelector(channel, onSelector, selecting);
                    } else if (type == Buttons::ButtonType::confirm) {
                        if (!onSelector) {
                            // exit channel selector, apply channel
                            if (prevChannel != channel) {
                                Serial.println("Applying Channel");

                                Communication::checkHC12Result(
                                        Communication::hc12.setChannel(channel),
                                        "Could not apply channel \n"
                                );

                                prevChannel = channel;
                            }

                            nextTask = reinterpret_cast<GUITask>(MainMenu::mainMenu);
                            return;
                        } else {
                            // change between navigation and channel selection
                            selecting = !selecting;
                            drawChannelSelector(channel, onSelector, selecting);
                        }
                    } else {
                        // region change channel
                        if (type == Buttons::ButtonType::previous && channel != 1) {
                            channel = static_cast<Channel>(channel - 1);
                            drawChannelSelector(channel, onSelector, selecting);
                        } else if (type == Buttons::ButtonType::next && channel != 100) {
                            channel = static_cast<Channel>(channel + 1);
                            drawChannelSelector(channel, onSelector, selecting);
                        }
                    }
                }
            });
        }

        return nextTask;
    }

    void drawChannelSelector(Channel currentChannel, bool onSelector, bool selecting) {
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
        Serial.println(currentChannel);

        if (!onSelector) {
            Serial.print("x");
        } else {
            Serial.print(" ");
        }

        Serial.println("  OK");

        Serial.println();
    }
}
