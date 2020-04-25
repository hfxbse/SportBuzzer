#include "sport_buzzer.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#define HC12_DEBUG_NAME String("HC12")

#define BUTTON 2
#define TIMEOUT 10000

HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);

volatile bool pingRequested = false;

void setup() {
    Serial.begin(9600);
    hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), []() {
        pingRequested = true;
    }, RISING);

    testHC12(HC12_DEBUG_NAME);
    setupHC12(HC12_DEBUG_NAME);
}

void loop() {
    static Transmissions transmissions(hc12, HC12_DEBUG_NAME);

    transmissions.poll();
    handlePingSignals(transmissions);
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
    static TransmissionStatus previousPingStatus = transmissions.getPingStatus();

    if (transmissions.popReceivedPing()) {
        transmissions.sendPingResponse();
    }

    if (pingRequested) {
        if (transmissions.sendPing(TIMEOUT)) {
            pingRequested = false;

            Serial.println("Sending ping on " + HC12_DEBUG_NAME + ".");
        }
    }

    const TransmissionStatus pingStatus = transmissions.getPingStatus();
    if (previousPingStatus != pingStatus) {
        if (pingStatus == finished) {
            Serial.println(
                    "Ping on " + HC12_DEBUG_NAME + " took " + String(transmissions.getPingResponseTime()) + "ms.");
        } else if (pingStatus == timeout) {
            Serial.println("Ping timeout on " + HC12_DEBUG_NAME + ".");
        }

        previousPingStatus = pingStatus;
    }
}