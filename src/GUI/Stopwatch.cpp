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
#include "LabeledString.hpp"

#define SECOND 1000
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

Stopwatch::Stopwatch(Transmissions &transmissions, unsigned long buzzerTime) {
    Stopwatch::buzzerTime = buzzerTime;
    stopwatchTime = transmissions.getStopwatchSignalTime();

    previousDurationNumber = transmissions.getDurationNumber();
    previousCancelNumber = transmissions.getCancelNumber();
}

GUITask *Stopwatch::update(
        Display &display,
        Transmissions &transmissions,
        unsigned long buzzerTime,
        bool redraw,
        uint16_t yOffset
) {
    if (redraw) {
        draw(display, yOffset, started ? 0 : duration);
        return this;
    }

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

                duration = buzzerTime - startTime;
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

            duration = stopwatchTime - startTime - transmissions.getPingResponseTime();
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
            duration = transmissions.getTransmittedDuration();
            draw(display, yOffset, duration);
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

    return this;
}

void Stopwatch::draw(Display &display, uint16_t yOffset, unsigned long duration) {
    uint16_t navigationHeight;

    if (!started) {
        const Option option("Verlassen", true);
        navigationHeight = drawNavigationBar(display, &option, 1);
    } else {
        const Option option("Abbrechen", true);
        navigationHeight = drawNavigationBar(display, &option, 1);
    }

    const uint16_t availableHeight = getAvailableHeight(navigationHeight + yOffset);

    const String label = started ? "Stoppuhr gestartet" : "Letzte Zeit";
    uint16_t width, height, labelHeight;

    display.setFontNow(Roboto_Thin7pt7b);
    display.getTextBounds("Stoppuhr", nullptr, &labelHeight);     // get maximal label height

    display.setFont(RobotoMono_Regular11pt7b);
    display.getTextBounds(getTimeString(0), &width, &height);

    const uint16_t margin = getMargin(7.5, availableHeight);
    const uint16_t requiredSpace = height + labelHeight + margin;
    const uint16_t upperBound = alignVertically(yOffset, availableHeight, requiredSpace) - margin;

    display.alignText(
            Display::right(50),
            upperBound + labelHeight + margin,
            width,
            height,
            TextAlign::centered
    );

    if (started || duration == 0) {
        display.print(getTimeString(duration, true));
    } else {
        display.print(getTimeString(duration));
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
