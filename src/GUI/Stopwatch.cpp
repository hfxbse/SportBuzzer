//
// Created by stubit on 7/16/20.
//

#include <HardwareSerial.h>
#include "Stopwatch.hpp"
#include "MainMenu.hpp"
#include "GUIInput.hpp"

GUITask *Stopwatch::update(Transmissions &transmissions, unsigned long buzzerTime, bool redraw) {
    if (redraw) {
        Stopwatch::buzzerTime = buzzerTime;
        stopwatchTime = transmissions.getStopwatchSignalTime();

        previousDurationNumber = transmissions.getDurationNumber();
        previousCancelNumber = transmissions.getCancelNumber();
        draw(0);
    } else {
        if (!started) {
            if (Stopwatch::buzzerTime != buzzerTime) {
                // start stopwatch on buzzer press if stopwatch
                started = true;
                Stopwatch::buzzerTime = buzzerTime;

                transmissions.sendStopwatchSignal();
                draw(0);
            } else if (transmissions.getStopwatchSignalTime() != stopwatchTime) {
                // start stopwatch on stopwatch signal
                started = true;
                stopwatchTime = transmissions.getStopwatchSignalTime();

                draw(0);
            }
        } else {
            unsigned long startTime = Stopwatch::buzzerTime > stopwatchTime ? Stopwatch::buzzerTime : stopwatchTime;

            if (Stopwatch::buzzerTime != buzzerTime) {
                // end stopwatch on buzzer press
                if (startTime == Stopwatch::buzzerTime) {
                    started = false;

                    unsigned long duration = buzzerTime - startTime;
                    transmissions.sendDuration(duration);
                    draw(duration);
                } else {
                    transmissions.sendStopwatchSignal();
                }

                Stopwatch::buzzerTime = buzzerTime;
            } else if (transmissions.getStopwatchSignalTime() != stopwatchTime) {
                // end stopwatch on stopwatch signal
                started = false;
                stopwatchTime = transmissions.getStopwatchSignalTime();

                unsigned long duration = stopwatchTime - startTime - transmissions.getPingResponseTime();
                transmissions.sendDuration(duration);
                draw(duration);
            }

            if (transmissions.getCancelNumber() != previousCancelNumber) {
                started = false;
                previousCancelNumber = transmissions.getCancelNumber();

                draw(0);
            }
        }

        if (transmissions.getDurationNumber() != previousDurationNumber) {
            // end stopwatch on received duration
            started = false;
            draw(transmissions.getTransmittedDuration());

            previousDurationNumber = transmissions.getDurationNumber();
        }

        GUIInput input;
        input.poll();

        if (input.confirm()) {
            if (!started) {
                return new MainMenu();
            } else {
                started = false;
                draw(0);

                transmissions.sendCancelSignal();
            }
        }
    }

    return this;
}

void Stopwatch::draw(unsigned long duration) {
    Serial.println("Stopwatch");
    Serial.print("Duration: ");
    Serial.println(duration);

    if (!started) {
        Serial.println("Press the buzzer to start.");
        Serial.println("Press either buzzer at any time to stop.");
        Serial.println("x  Exit");
    } else {
        Serial.println("Stopwatch is running.");
        Serial.println("Press either buzzer at any time to stop.");
        Serial.println("x  Cancel");
    }

    Serial.println();
}
