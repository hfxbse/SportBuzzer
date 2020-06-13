#include "Transmissions.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#define HC12_DEBUG_NAME String("HC12")

#define BUTTON 2
#define TIMEOUT 10000

void checkHC12Result(bool result, const String &msg);

void testHC12(const String& debugName);

void setupHC12(const String &debugName);

void handlePingSignals(Transmissions &transmissions);

unsigned long getPingDuration(Transmissions &transmissions);

HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);

volatile unsigned long timerStart = 0;

void setup() {
    Serial.begin(9600);
    hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), []() {
        timerStart = millis();
    }, RISING);

    testHC12(HC12_DEBUG_NAME);
    setupHC12(HC12_DEBUG_NAME);
}

void loop() {
    static Transmissions transmissions(hc12, HC12_DEBUG_NAME);
    static unsigned long oldTimerStart = timerStart;

    transmissions.poll();
    handlePingSignals(transmissions);

    // region buzzer press handler
    static unsigned long oldReceiveTime = transmissions.getBuzzerReceiveTime();
    const unsigned long receiveTime = transmissions.getBuzzerReceiveTime();

    if(oldTimerStart != timerStart) {
        oldTimerStart = timerStart;
        transmissions.sendBuzzerSignal();
    }

    if(oldReceiveTime != receiveTime) {
        oldReceiveTime = receiveTime;
        unsigned long duration = receiveTime - timerStart - getPingDuration(transmissions);

        Serial.println("Duration: " + String(duration) + "ms.");
        transmissions.sendDuration(duration);
    }
    // endregion

    // region received duration
    static unsigned long oldDurationNumber = transmissions.getDurationNumber();
    const unsigned long durationNumber = transmissions.getDurationNumber();

    if(oldDurationNumber != durationNumber) {
        oldDurationNumber = durationNumber;
        Serial.println("Received duration: " + String(transmissions.getTransmittedDuration()) + "ms.");
    }
    // endregion
}

void checkHC12Result(bool result, const String &msg) {
    if (!result) {
        Serial.println(msg);
    }
}

void testHC12(const String &debugName) {
    checkHC12Result(hc12.test(), "Unable to connect to " + debugName + ".");
}

void setupHC12(const String &debugName) {
    checkHC12Result(hc12.restoreDefaults(), "Unable to restore defaults on " + debugName + ".");

    Serial.println("Setup completed on " + debugName);
}

void handlePingSignals(Transmissions &transmissions) {
    if (transmissions.popReceivedPing()) {
        transmissions.sendPingResponse();
    }
}

unsigned long getPingDuration(Transmissions &transmissions) {
    if(transmissions.sendPing(TIMEOUT)) {
        Serial.println("Send ping on " + HC12_DEBUG_NAME + ".");
    }

    transmissions.waitOnPing();
    TransmissionStatus pingStatus = transmissions.getPingStatus();

    if (pingStatus == finished) {
        Serial.println("Ping on " + HC12_DEBUG_NAME + " took " + String(transmissions.getPingResponseTime()) + "ms.");
    } else if (pingStatus == timeout) {
        Serial.println("Ping timeout on " + HC12_DEBUG_NAME + ".");
    }

    return transmissions.getPingResponseTime();
}