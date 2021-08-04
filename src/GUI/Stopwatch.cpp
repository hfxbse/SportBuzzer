//
// Created by stubit on 7/16/20.
//

#include <HardwareSerial.h>
#include "Stopwatch.hpp"
#include "MainMenu.hpp"
#include "GUIInput.hpp"
#include "NavigationBar.hpp"

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
        draw(display, 0);
    } else {
        if (!started) {
            if (Stopwatch::buzzerTime != buzzerTime) {
                // start stopwatch on buzzer press if stopwatch
                started = true;
                Stopwatch::buzzerTime = buzzerTime;

                transmissions.sendStopwatchSignal();
                draw(display, 0);
                display.update();
            } else if (transmissions.getStopwatchSignalTime() != stopwatchTime) {
                // start stopwatch on stopwatch signal
                started = true;
                stopwatchTime = transmissions.getStopwatchSignalTime();

                draw(display, 0);
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
                    draw(display, duration);
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
                draw(display, duration);
                display.update();
            }

            if (transmissions.getCancelNumber() != previousCancelNumber) {
                previousCancelNumber = transmissions.getCancelNumber();

                if (transmissions.getCancelSignal() == Signal::cancel_stopwatch) {
                    started = false;
                    draw(display, 0);
                    display.update();
                }
            }
        }

        if (transmissions.getDurationNumber() != previousDurationNumber) {
            previousDurationNumber = transmissions.getDurationNumber();

            if (transmissions.getDurationSignal() == Signal::duration_stopwatch) {
                started = false;
                draw(display, transmissions.getTransmittedDuration());
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
                draw(display, 0);
                display.update();

                transmissions.sendCancelSignal(true);
            }
        }
    }

    return this;
}

void Stopwatch::draw(Display &display, unsigned long duration) {
    Serial.println("Stopwatch");
    Serial.print("Duration: ");
    // region draw last time recorded by stopwatch
    Serial.println(duration);
    // endregion

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
        drawNavigationBar(display, &option, 1);
        // endregion
    } else {
        // region draw info when own stopwatch is running
        Serial.println("Stopwatch is running.");
        Serial.println("Press either buzzer at any time to stop.");
        Serial.println("x  Cancel");

        const Option option("Abbrechen", true);
        drawNavigationBar(display, &option, 1);
        // endregion
    }

    Serial.println();
}
