//
// Created by stubit on 4/25/20.
//

#include "Transmissions.hpp"

Transmissions::Transmissions(HC12 &module, const String &debugName) : module(module), moduleDebugName(debugName) {}

void Transmissions::poll() {
    while (module.available()) {
        switch (module.read()) {
            case ping:
                ++receivedPingCount;
                break;

            case pong:
                pingStatus = finished;
                pingResponseTime = millis() - pingStart;
                break;

            case buzzer:
                buzzerTime = millis();
                break;
        }
    }

    if(pingStatus == unfinished && millis() - pingStart > pingTimeout) {
        pingStatus = timeout;
    }
}

bool Transmissions::sendPing(unsigned long timeout) {
    if(pingStatus != unfinished) {
        pingStatus = unfinished;
        Transmissions::pingTimeout = timeout;

        pingStart = millis();

        module.write(ping);

        return true;
    }

    return false;
}

TransmissionStatus Transmissions::getPingStatus() {
    return pingStatus;
}

unsigned long Transmissions::getPingResponseTime() {
    waitOnPing();
    return pingStatus == finished ? pingResponseTime : 0;
}

unsigned long Transmissions::popReceivedPing() {
    if(receivedPingCount) {
        return receivedPingCount--;
    }

    return 0;
}

void Transmissions::sendPingResponse() {
    module.write(pong);
}

void Transmissions::waitOnPing() {
    while (pingStatus == unfinished) {
        poll();
    }
}

void Transmissions::sendBuzzerSignal() {
    module.write(buzzer);
}

unsigned long Transmissions::getBuzzerReceiveTime() const {
    return buzzerTime;
}
