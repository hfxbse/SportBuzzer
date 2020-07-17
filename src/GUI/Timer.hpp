//
// Created by stubit on 7/17/20.
//

#ifndef SPORTBUZZER_TIMER_HPP
#define SPORTBUZZER_TIMER_HPP


#include "GUITask.hpp"

class Timer : public GUITask {
public:
    GUITask *update(Transmissions &transmissions, unsigned long buzzerTime, bool redraw) override;

private:
    void draw();

    static String getNumberString(unsigned int number);

    void prepareTimerStart();

    // related to timer
    long leftTime = 0;
    long previousLeftTimeNumber;
    unsigned long buzzerTime, timerSignalTime;
    unsigned int timeLimit = 1;
    byte previousLimitNumber;

    // related to navigation
    bool started = false, onLimit = true, changingLimit = false;
    byte digitOffset = 0;
};


#endif //SPORTBUZZER_TIMER_HPP
