//
// Created by stubit on 7/17/20.
//


#include "Timer.hpp"
#include "GUIInput.hpp"
#include "MainMenu.hpp"
#include "NavigationBar.hpp"
#include "fonts/RobotoMono_Regular11pt7b.h"
#include "fonts/Roboto_Thin7pt7b.h"
#include "fonts/RobotoMono_Thin7pt7b.h"
#include "LabeledString.hpp"
#include "NumberInput.hpp"

GUITask *Timer::update(
        Display &display,
        Transmissions &transmissions,
        unsigned long buzzerTime,
        bool redraw,
        uint16_t yOffset
) {
    if (redraw) {
        timerSignalTime = transmissions.getTimerSignalTime();
        Timer::buzzerTime = buzzerTime;
        Timer::previousLeftTimeNumber = transmissions.getDurationNumber();
        previousLimitNumber = transmissions.getLimitNumber();
        previousCancelNumber = transmissions.getCancelNumber();
        draw(display, yOffset);
    } else {
        // region buzzer
        if (Timer::buzzerTime != buzzerTime) {
            started = !started;

            if (started) {
                canceled = false;
                prepareTimerStart();

                transmissions.sendTimerSignal();
                transmissions.sendLimit(timeLimit);

                redraw = true;
            } else {
                if (Timer::buzzerTime > timerSignalTime) {
                    leftTime = timeLimit - (buzzerTime - Timer::buzzerTime);

                    sendTimes(transmissions);
                } else {
                    transmissions.sendTimerSignal();
                    transmissions.sendLimit(0);
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

            if (transmissions.getDurationSignal() == Signal::duration_timer) {
                leftTime = static_cast<long>(transmissions.getTransmittedDuration());
                started = false;
                canceled = false;
            }
        }

        if (previousLimitNumber != transmissions.getLimitNumber()) {
            // redraw with new time limit
            previousLimitNumber = transmissions.getLimitNumber();

            bool ignore = limitChangeCooldown && transmissions.getTransmittedLimit() > 0;

            if (!ignore && (!started || (Timer::buzzerTime < timerSignalTime))) {
                timeLimit = transmissions.getTransmittedLimit();
                timeLimit *= timeLimit < 0 ? -1 : 1;
            }

            redraw = redraw || !ignore;
            limitChangeCooldown = false;
        }

        if (transmissions.getTimerSignalTime() != timerSignalTime) {
            if (!started) {
                // start on timer signal if not started
                prepareTimerStart();
            } else {
                // stop on timer signal if started
                started = false;
                unsigned long startTime = timerSignalTime > buzzerTime ? timerSignalTime : buzzerTime;

                leftTime = timeLimit - (transmissions.getTimerSignalTime() - startTime);

                sendTimes(transmissions);

                redraw = true;
            }

            timerSignalTime = transmissions.getTimerSignalTime();
        }

        if (previousCancelNumber != transmissions.getCancelNumber()) {
            previousCancelNumber = transmissions.getCancelNumber();

            if (transmissions.getCancelSignal() == Signal::cancel_timer) {
                redraw = redraw || started;

                started = false;
                leftTime = 0;
                canceled = true;
            }
        }
        // endregion

        GUIInput input;
        input.poll();

        // region navigation
        if (started && input.confirm()) {
            started = false;
            leftTime = 0;
            canceled = true;

            transmissions.sendCancelSignal(false);

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

        if (redraw) {   // redraw is not a forced full redraw in this case
            draw(display, yOffset);
            display.update();
        }
    }

    return this;
}

void Timer::draw(Display &display, uint16_t yOffset) {
    Serial.println("Timer");

    Serial.print("Time left: ");
    Serial.println(leftTime);

    if (!started) {
        // draw info if own timer is not running
        Serial.println("Press the buzzer to Start");
    } else {
        // draw info if own timer is running
        Serial.println("Timer is running.");
    }

    // --- IMPORTANT ---
    //
    // IF ON ANOTHER BUZZER WITHIN THE SAME WIRELESS CHANNEL HAS A TIMER CURRENTLY RUNNING,
    // PRESSING THE BUZZER WILL STOP IT REGARDLESS OF ITS OWN STATE
    //
    Serial.println("Press either buzzer at any time to stop.");

    if (started) {
        // indicate input field being disabled while the timer is running
        Serial.print("-");
    } else if (changingLimit) {
        // print current position of digit which is getting modified, if the time limit is getting modified
        Serial.print(digitOffset + 1);
    } else if (onLimit) {
        // show cursor on time limit field, but it is not getting modified
        Serial.print("x");
    } else {
        // time limit is not getting modified and the cursor is not on the input field
        Serial.print(" ");
    }

    const Option options[2] = {
            Option(started ? "Abbrechen" : "Verlassen", started || !onLimit),
            Option("Umstellen", !started && onLimit && !changingLimit)
    };

    int16_t navigationHeight = drawNavigationBar(display, options, 1 + !started);

    // region draw borders
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
    // endregion draw borders

    // draw labeled time
    const uint16_t availableHeight = getAvailableHeight(yOffset + navigationHeight);

    const String label = started ? "Timer gestartet" : "Verbliebene Zeit";
    uint16_t width, height, labelHeight, limitHeight, limitWidth;

    display.setFontNow(Roboto_Thin7pt7b);
    display.getTextBounds("Timer gestartet", nullptr, &labelHeight);     // get maximal label height

    display.setFontNow(RobotoMono_Thin7pt7b);
    display.getTextBounds(getTimeString(0), &limitWidth, &limitHeight);     // get maximal limit height

    display.setFont(RobotoMono_Regular11pt7b);
    display.getTextBounds("-" + getTimeString(0), &width, &height);

    const uint16_t labelMargin = getMargin(7.5, availableHeight);
    const uint16_t limitMargin = getMargin(15, availableHeight);
    const uint16_t requiredSpace = height + labelHeight + limitHeight + labelMargin + limitMargin;

    uint16_t upperBound = alignVertically(yOffset, availableHeight, requiredSpace);
    upperBound -= getMargin(2.5, availableHeight);

    display.alignText(
            Display::right(50),
            upperBound + labelHeight + labelMargin,
            width,
            height,
            TextAlign::centered
    );

    if (started || canceled) {
        display.print("-" + getTimeString(0, true));
    } else {
        display.print(
                (leftTime < 0 ? "-" : "+") +
                getTimeString(leftTime < 0 ? -1 * leftTime : leftTime)
        );
    }

    display.setFont(Roboto_Thin7pt7b);
    display.alignText(
            Display::right(50),
            upperBound,
            width,  // use time with to get same horizontal placement
            labelHeight,
            TextAlign::centered
    );

    display.print(label);
    // endregion draw labeled time

    // region draw limit
    const uint16_t leftBorder = Display::right(50) + ((static_cast<float>(width) / 2) + 0.5);
    const uint16_t limitY = upperBound + labelMargin + labelHeight + height + limitMargin;

    display.setFont(RobotoMono_Thin7pt7b);
    display.alignText(leftBorder, limitY, limitWidth, limitHeight, TextAlign::right);

    display.print(getTimeString(timeLimit));

    // region draw cursor
    if (changingLimit) {
        const String limit = getTimeString(0);
        byte separatorCount = digitOffset > 7 ? 4 : (digitOffset / 2);

        drawCursor(
                display,
                limit,
                getTimeString(timeLimit),
                separatorCount + digitOffset,
                leftBorder,
                limitHeight,
                limitY,
                availableHeight
        );
    }
    // endregion draw cursor

    display.setFont(Roboto_Thin7pt7b);
    display.getTextBounds("Limit", &width, nullptr);

    display.alignText(
            leftBorder - limitWidth - Display::left(2.5),
            limitY,
            width,
            limitHeight,
            TextAlign::right
    );

    display.print("Limit");


    // endregion draw limit

    // region print current time limit
    Serial.println("  Time limit: " + getTimeString(timeLimit));
    // endregion

    // region draw button
    if (!started) {
        if (onLimit) {  // cursor is not on the button
            Serial.print(" ");
        } else {    // cursor is on the button
            Serial.print("x");
        }

        Serial.println("  Exit");
    } else {
        Serial.println("x  Cancel");
    }
    // endregion

    Serial.println();
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
    // region determine digit limits and step sizes
    long stepSize, nextBiggerStepSize;
    byte digitLimit = 9, upperDigitLimit = 5, lowerDigitLimit = 9;

    if (digitOffset < 2) {
        stepSize = DAY;
        nextBiggerStepSize = DAY * 10;

        upperDigitLimit = 2;
        lowerDigitLimit = 4;
    } else if (digitOffset < 4) {
        stepSize = HOUR;
        nextBiggerStepSize = DAY;

        upperDigitLimit = 2;
        lowerDigitLimit = 3;
    } else if (digitOffset < 6) {
        stepSize = MINUTE;
        nextBiggerStepSize = HOUR;
    } else if (digitOffset < 8) {
        stepSize = SECOND;
        nextBiggerStepSize = MINUTE;
    } else {
        stepSize = pow(10, 10 - digitOffset);
        nextBiggerStepSize = pow(10, 11 - digitOffset);
    }

    long limit = timeLimit;
    limit -= (limit / nextBiggerStepSize) * nextBiggerStepSize;

    if (digitOffset < 8) {
        if (digitOffset % 2 == 0) {
            stepSize *= 10;
            digitLimit = upperDigitLimit;
        } else if (limit / (stepSize * 10) == upperDigitLimit) {
            digitLimit = lowerDigitLimit;
        }
    }
    // endregion

    // region apply input while enforcing digit and absolute limits
    if (input.previous() && (timeLimit / stepSize) % 10 != 0) {
        if (timeLimit - stepSize > 0) {
            timeLimit -= stepSize;
            return true;
        }
    } else if (input.next() && (limit / stepSize) % 10 != digitLimit) {
        if (timeLimit + stepSize > 0) {
            timeLimit += stepSize;
            return true;
        }
    } else if (input.confirm()) {
        ++digitOffset;

        if (digitOffset >= 11) {    // all 11 digits have been focused
            changingLimit = false;
            timeLimit = timeLimit > 0 ? timeLimit : 1;
            // negative value marks manual input
            transmissions.sendLimit(timeLimit * -1);
        }

        return true;
    }
    // endregion

    return false;
}

void Timer::sendTimes(Transmissions &transmissions) {
    limitChangeCooldown = true;

    transmissions.sendDuration(leftTime, false);
    transmissions.sendLimit(timeLimit);
}
