//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_CHANNEL_SELECTOR_HPP
#define SPORTBUZZER_CHANNEL_SELECTOR_HPP

#include "../HardwareInterfaces/HC12_Options.hpp"
#include "GUITask.hpp"

class ChannelSelector : public GUITask {
public:
    GUITask * update(bool redraw);

private:
    void draw();

    static inline Channel channel = C001;
    const Channel previousChannel = channel;

    bool onSelector = true, selecting = false;
};

#endif //SPORTBUZZER_CHANNEL_SELECTOR_HPP
