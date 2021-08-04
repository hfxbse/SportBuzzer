//
// Created by stubit on 7/17/20.
//

#ifndef SPORTBUZZER_TIMER_HPP
#define SPORTBUZZER_TIMER_HPP


#include "GUITask.hpp"
#include "GUIInput.hpp"

class Timer : public GUITask {
public:
    GUITask *update(Display &display, Transmissions &transmissions, unsigned long buzzerTime, bool redraw,
                    uint16_t yOffset) override;

private:
    void draw();

    static String getNumberString(unsigned int number);

    void prepareTimerStart();

    bool timeLimitInput(const GUIInput &input, const Transmissions &transmissions);

    void sendTimes(Transmissions &transmissions);

    // related to timer
    long leftTime = 0;
    long previousLeftTimeNumber;
    unsigned long buzzerTime, timerSignalTime;
    long timeLimit = 1;
    byte previousLimitNumber, previousCancelNumber;
    bool limitChangeCooldown = false;

    // related to navigation
    bool started = false, onLimit = true, changingLimit = false;
    byte digitOffset = 0;
};


#endif //SPORTBUZZER_TIMER_HPP
