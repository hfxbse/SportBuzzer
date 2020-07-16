//
// Created by stubit on 7/16/20.
//

#ifndef SPORTBUZZER_STOPWATCH_HPP
#define SPORTBUZZER_STOPWATCH_HPP


#include "GUITask.hpp"

class Stopwatch : public GUITask {
public:
    GUITask *update(Transmissions &transmissions, unsigned long buzzerTime, bool redraw) override;

private:
    void draw(unsigned long duration);

    bool started = false;
    unsigned long buzzerTime, stopwatchTime;
    byte previousDurationNumber, previousCancelNumber;
};


#endif //SPORTBUZZER_STOPWATCH_HPP
