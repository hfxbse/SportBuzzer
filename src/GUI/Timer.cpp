//
// Created by stubit on 7/17/20.
//

#include "Timer.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"

GUITask *Timer::update(Transmissions &transmissions, unsigned long buzzerTime, bool redraw) {
    if (redraw) {
        timerSignalTime = transmissions.getTimerSignalTime();
        Timer::buzzerTime = buzzerTime;
        Timer::previousLeftTimeNumber = transmissions.getDurationNumber();
        previousLimitNumber = transmissions.getLimitNumber();
        previousCancelNumber = transmissions.getCancelNumber();
        draw();
    } else {
        // region buzzer
        if (Timer::buzzerTime != buzzerTime) {
            started = !started;

            if (started) {
                prepareTimerStart();

                transmissions.sendTimerSignal();
                transmissions.sendLimit(timeLimit);

                redraw = true;
            } else {
                if (Timer::buzzerTime > timerSignalTime) {
                    leftTime = (timeLimit * 1000) - (buzzerTime - Timer::buzzerTime);

                    transmissions.sendDuration(leftTime);
                    transmissions.sendLimit(timeLimit);
                } else {
                    transmissions.sendTimerSignal();
                }

                redraw = Timer::buzzerTime > timerSignalTime;
            }

            Timer::buzzerTime = buzzerTime;
        }
        // endregion

        // region communication
        if (previousLeftTimeNumber != transmissions.getDurationNumber()) {
            // stop timer on received duration, apply duration
            previousLeftTimeNumber = transmissions.getDurationNumber();

            leftTime = static_cast<long>(transmissions.getTransmittedDuration());
            started = false;
        }

        if (previousLimitNumber != transmissions.getLimitNumber()) {
            // redraw with new time limit
            previousLimitNumber = transmissions.getLimitNumber();

            if (!started || (started && Timer::buzzerTime < timerSignalTime)) {
                timeLimit = transmissions.getTransmittedLimit();
            }

            redraw = true;
        }

        if (transmissions.getTimerSignalTime() != timerSignalTime) {
            if (!started) {
                // start on timer signal if not started
                prepareTimerStart();
            } else {
                // stop on timer signal if started
                started = false;
                unsigned long startTime = timerSignalTime > buzzerTime ? timerSignalTime : buzzerTime;

                leftTime = (timeLimit * 1000) - (transmissions.getTimerSignalTime() - startTime);

                transmissions.sendDuration(leftTime);
                transmissions.sendLimit(timeLimit);

                redraw = true;
            }

            timerSignalTime = transmissions.getTimerSignalTime();
        }

        if (previousCancelNumber != transmissions.getCancelNumber()) {
            redraw = redraw || started;

            started = false;
            leftTime = 0;

            previousCancelNumber = transmissions.getCancelNumber();
        }
        // endregion

        GUIInput input;
        input.poll();

        // region navigation
        if (started && input.confirm()) {
            started = false;
            leftTime = 0;

            transmissions.sendCancelSignal();

            redraw = true;
        } else if (!started) {
            if (!changingLimit && (input.next() || input.previous())) {
                onLimit = !onLimit;
                redraw = true;
            } else if (input.confirm() && !changingLimit) {
                if (onLimit) {
                    changingLimit = true;
                    digitOffset = 0;
                    redraw = true;
                } else {
                    return new MainMenu();
                }
            } else if (changingLimit) {
                redraw = redraw || timeLimitInput(input, transmissions);
            }
        }
        // endregion

        if (redraw) {
            draw();
        }
    }

    return this;
}

void Timer::draw() {
    Serial.println("Timer");

    Serial.print("Time left: ");
    Serial.println(leftTime);

    if (!started) {
        Serial.println("Press the buzzer to Start");
    } else {
        Serial.println("Timer is running.");
    }

    Serial.println("Press either buzzer at any time to stop.");

    unsigned int hours = timeLimit / 3600;
    unsigned int minutes = (timeLimit - hours * 3600) / 60;
    unsigned int seconds = timeLimit % 60;

    if (started) {
        Serial.print("-");
    } else if (changingLimit) {
        Serial.print(digitOffset + 1);
    } else if (onLimit) {
        Serial.print("x");
    } else {
        Serial.print(" ");
    }

    Serial.print("  Time limit: ");
    Serial.print(hours);
    Serial.print(":");
    Serial.print(getNumberString(minutes));
    Serial.print(":");
    Serial.println(getNumberString(seconds));

    if (!started) {
        if (onLimit) {
            Serial.print(" ");
        } else {
            Serial.print("x");
        }

        Serial.println("  Exit");
    } else {
        Serial.println("x  Cancel");
    }

    Serial.println();
}

String Timer::getNumberString(unsigned int number) {
    String string = String(number);

    while (string.length() < 2) {
        string = "0" + string;
    }

    return string;
}

void Timer::prepareTimerStart() {
    started = true;
    changingLimit = false;
    onLimit = false;
    leftTime = 0;
    timeLimit = timeLimit > 1 ? timeLimit : 1;
    digitOffset = 0;
}

bool Timer::timeLimitInput(const GUIInput &input, const Transmissions &transmissions) {
    unsigned stepSize = digitOffset % 2 ? 10 * pow(60, digitOffset / 2) : pow(60, digitOffset / 2);
    stepSize += digitOffset >= 2 ? 1 : 0;

    if (input.previous() && (timeLimit / stepSize) % 10 != 0) {
        timeLimit -= stepSize;
        return true;
    } else if (input.next() && (timeLimit / stepSize) % 10 != (digitOffset % 2 ? 5 : 9)) {
        timeLimit += stepSize;
        return true;
    } else if (input.confirm()) {
        ++digitOffset;

        if (digitOffset > 4) {
            changingLimit = false;
            timeLimit = timeLimit > 0 ? timeLimit : 1;
            transmissions.sendLimit(timeLimit);
        }

        return true;
    }

    return false;
}
