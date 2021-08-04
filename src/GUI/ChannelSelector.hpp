//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_CHANNEL_SELECTOR_HPP
#define SPORTBUZZER_CHANNEL_SELECTOR_HPP

#include <src/HardwareInterfaces/HC12_Options.hpp>
#include "GUITask.hpp"

class ChannelSelector : public GUITask {
public:
    GUITask *update(Display &display, Transmissions &, unsigned long, bool redraw, uint16_t yOffset) override;

    static String currentChannel(Channel channel = ChannelSelector::channel);

private:
    void draw(Display &display);

    static inline Channel channel = C001;
    Channel previousChannel = channel;

    bool onSelector = true, selecting = false;

    byte digitOffset = 0;
};

#endif //SPORTBUZZER_CHANNEL_SELECTOR_HPP
