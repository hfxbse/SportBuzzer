//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_CHANNEL_SELECTOR_HPP
#define SPORTBUZZER_CHANNEL_SELECTOR_HPP

#include "../HardwareInterfaces/HC12_Options.hpp"
#include "GUITask.hpp"

namespace ChannelSelector {
    GUITask channelSelector(bool redraw);

    void drawChannelSelector(Channel currentChannel, bool onSelector, bool selecting);
}

#endif //SPORTBUZZER_CHANNEL_SELECTOR_HPP
