//
// Created by stubit on 7/16/20.
//

#ifndef SPORTBUZZER_STOPWATCH_HPP
#define SPORTBUZZER_STOPWATCH_HPP


#include "GUITask.hpp"

class Stopwatch : public GUITask {
public:
    Stopwatch(Transmissions &transmissions, unsigned long buzzerTime);

    GUITask *update(
            Display &display,
            Transmissions &transmissions,
            unsigned long buzzerTime,
            bool redraw,
            uint16_t yOffset
    ) override;

private:
    void draw(Display &display, uint16_t yOffset, unsigned long duration);

    bool started = false;
    unsigned long buzzerTime, stopwatchTime, duration = 0;
    byte previousDurationNumber, previousCancelNumber;
};


#endif //SPORTBUZZER_STOPWATCH_HPP
