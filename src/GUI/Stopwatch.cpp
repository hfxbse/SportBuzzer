//
// Created by stubit on 7/16/20.
//

#include <HardwareSerial.h>
#include "Stopwatch.hpp"
#include "MainMenu.hpp"
#include "GUIInput.hpp"
#include "NavigationBar.hpp"
#include "fonts/Roboto_Thin7pt7b.h"
#include "fonts/RobotoMono_Regular11pt7b.h"

#define SECOND 1000
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

String digits(uint16_t number, size_t min = 2) {
    String string = String(number);

    while (string.length() < min) {
        string = "0" + string;
    }

    return string;
}

GUITask *Stopwatch::update(
        Display &display,
        Transmissions &transmissions,
        unsigned long buzzerTime,
        bool redraw,
        uint16_t yOffset
) {
    if (redraw) {
        Stopwatch::buzzerTime = buzzerTime;
        stopwatchTime = transmissions.getStopwatchSignalTime();

        previousDurationNumber = transmissions.getDurationNumber();
        previousCancelNumber = transmissions.getCancelNumber();
        draw(display, yOffset, 0);
    } else {
        if (!started) {
            if (Stopwatch::buzzerTime != buzzerTime) {
                // start stopwatch on buzzer press if stopwatch
                started = true;
                Stopwatch::buzzerTime = buzzerTime;

                transmissions.sendStopwatchSignal();
                draw(display, yOffset, 0);
                display.update();
            } else if (transmissions.getStopwatchSignalTime() != stopwatchTime) {
                // start stopwatch on stopwatch signal
                started = true;
                stopwatchTime = transmissions.getStopwatchSignalTime();

                draw(display, yOffset, 0);
                display.update();
            }
        } else {
            unsigned long startTime = Stopwatch::buzzerTime > stopwatchTime ? Stopwatch::buzzerTime : stopwatchTime;

            if (Stopwatch::buzzerTime != buzzerTime) {
                // end stopwatch on buzzer press
                if (startTime == Stopwatch::buzzerTime) {
                    started = false;

                    unsigned long duration = buzzerTime - startTime;
                    transmissions.sendDuration(duration, true);
                    draw(display, yOffset, duration);
                    display.update();
                } else {
                    transmissions.sendStopwatchSignal();
                }

                Stopwatch::buzzerTime = buzzerTime;
            } else if (transmissions.getStopwatchSignalTime() != stopwatchTime) {
                // end stopwatch on stopwatch signal
                started = false;
                stopwatchTime = transmissions.getStopwatchSignalTime();

                unsigned long duration = stopwatchTime - startTime - transmissions.getPingResponseTime();
                transmissions.sendDuration(duration, true);
                draw(display, yOffset, duration);
                display.update();
            }

            if (transmissions.getCancelNumber() != previousCancelNumber) {
                previousCancelNumber = transmissions.getCancelNumber();

                if (transmissions.getCancelSignal() == Signal::cancel_stopwatch) {
                    started = false;
                    draw(display, yOffset, 0);
                    display.update();
                }
            }
        }

        if (transmissions.getDurationNumber() != previousDurationNumber) {
            previousDurationNumber = transmissions.getDurationNumber();

            if (transmissions.getDurationSignal() == Signal::duration_stopwatch) {
                started = false;
                draw(display, yOffset, transmissions.getTransmittedDuration());
                display.update();
            }
        }

        GUIInput input;
        input.poll();

        if (input.confirm()) {
            if (!started) {
                return new MainMenu();
            } else {
                started = false;
                draw(display, yOffset, 0);
                display.update();

                transmissions.sendCancelSignal(true);
            }
        }
    }

    return this;
}

void Stopwatch::draw(Display &display, uint16_t yOffset, unsigned long duration) {
    Serial.println("Stopwatch");
    Serial.print("Duration: ");
    // region draw last time recorded by stopwatch
    Serial.println(duration);
    // endregion

    uint16_t navigationHeight;

    if (!started) {
        // region draw info when own stopwatch is not running
        Serial.println("Press the buzzer to start.");

        // --- IMPORTANT ---
        //
        // IF ON ANOTHER BUZZER WITHIN THE SAME WIRELESS CHANNEL HAS A STOPWATCH CURRENTLY RUNNING,
        // PRESSING THE BUZZER WILL STOP IT REGARDLESS OF ITS OWN STATE
        //
        Serial.println("Press either buzzer at any time to stop.");
        Serial.println("x  Exit");

        const Option option("Verlassen", true);
        navigationHeight = drawNavigationBar(display, &option, 1);
        // endregion
    } else {
        // region draw info when own stopwatch is running
        Serial.println("Stopwatch is running.");
        Serial.println("Press either buzzer at any time to stop.");
        Serial.println("x  Cancel");

        const Option option("Abbrechen", true);
        navigationHeight = drawNavigationBar(display, &option, 1);
        // endregion
    }

    const uint16_t availableHeight = Display::bottom(0) - Display::top(0) - navigationHeight - yOffset + 1;

    const String label = started ? "Stoppuhr gestartet" : "Letzte Zeit";
    uint16_t width, height, labelHeight;

    display.setFont(Roboto_Thin7pt7b);
    display.getTextBounds("Stoppuhr", nullptr, &labelHeight);     // get maximal label height

    display.setFont(RobotoMono_Regular11pt7b);
    display.getTextBounds("00:00:00:00:000", &width, &height);

    const uint16_t margin = Display::top(7.5, availableHeight) - Display::top(0);
    const uint16_t requiredSpace = height + labelHeight + margin;
    const uint16_t upperBound = Display::top(0) + yOffset + (
            static_cast<float>(availableHeight - requiredSpace) / 2 + .5
    ) - (1.5f * margin + 0.5);

    display.alignText(
            Display::right(50),
            upperBound + labelHeight + margin,
            width,
            height,
            TextAlign::centered
    );

    if (started || duration == 0) {
        display.print("--:--:--:--:---");
    } else {
        const uint16_t days = duration / (DAY);
        duration -= DAY * days;

        const uint16_t hours = duration / HOUR;
        duration -= HOUR * hours;

        const uint16_t minutes = duration / MINUTE;
        duration -= MINUTE * minutes;

        const uint16_t seconds = duration / SECOND;
        duration -= SECOND * seconds;

        display.print(
                digits(days) + ":" +
                digits(hours) + ":" +
                digits(minutes) + ":" +
                digits(seconds) + ":" +
                digits(duration, 3)
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

    Serial.println();

    display.drawLine(
            Display::right(0),
            Display::top(0) + yOffset,
            Display::right(0),
            Display::bottom(0) - navigationHeight
    );

    display.drawLine(
            Display::left(0),
            Display::top(0) + yOffset,
            Display::left(0),
            Display::bottom(0) - navigationHeight
    );
}
