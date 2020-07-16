#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"

#include "GUI/MainMenu.hpp"
#include "Wireless/Communication.hpp"

#define BUTTON 2
#define TIMEOUT 2000

volatile unsigned long timerStart = 0;

void setup() {
    Serial.begin(9600);

    Communication::hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), []() {
        timerStart = millis();
    }, RISING);

    Communication::testHC12();
    Communication::setupHC12();

    Communication::drawConnectionStatus(TransmissionStatus::timeout);
}

void loop() {
    static GUITask * task = new MainMenu();
    static GUITask * prevTask = nullptr;

    // region update battery status
    // TODO
    // endregion

    // region gui task handler
    auto newTask = task->update(task != prevTask);
    if (newTask != nullptr) {
        prevTask = task;
        task = newTask;

        if(prevTask != task) {
            delete prevTask;
        }
    } else {
        Serial.println("ERROR: No next task provided after finishing the current task.");
        Serial.println();
    }
    // endregion

    static Transmissions transmissions(Communication::hc12);
    static unsigned long oldTimerStart = timerStart;

    transmissions.poll();
    Communication::handlePingSignals(transmissions);

    // region check connection
    if (millis() - transmissions.getPingTime() > TIMEOUT + 100) {
        transmissions.sendPing(TIMEOUT);
    }

    Communication::drawConnectionStatus(transmissions.getPingStatus());
    // endregion

    // region buzzer press handler
    static unsigned long oldReceiveTime = transmissions.getBuzzerReceiveTime();
    const unsigned long receiveTime = transmissions.getBuzzerReceiveTime();

    if (oldTimerStart != timerStart) {
        oldTimerStart = timerStart;
        transmissions.sendBuzzerSignal();
    }

    if (oldReceiveTime != receiveTime) {
        oldReceiveTime = receiveTime;
        unsigned long duration = receiveTime - timerStart - Communication::getPingDuration(transmissions);

        Serial.println("Duration: " + String(duration) + "ms.");
        transmissions.sendDuration(duration);
    }
    // endregion

    // region received duration
    static unsigned long oldDurationNumber = transmissions.getDurationNumber();
    const unsigned long durationNumber = transmissions.getDurationNumber();

    if (oldDurationNumber != durationNumber) {
        oldDurationNumber = durationNumber;
        Serial.println("Received duration: " + String(transmissions.getTransmittedDuration()) + "ms.");
    }
    // endregion
}

#pragma clang diagnostic pop
