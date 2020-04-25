//
// Created by stubit on 4/25/20.
//

#include "Transmissions.hpp"

Transmissions::Transmissions(HC12 &module, const String &debugName) : module(module), debugName(debugName) {}

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

TransmissionStatus Transmissions::getPingStatus() const {
    return pingStatus;
}

unsigned long Transmissions::getPingResponseTime() const {
    return pingResponseTime;
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
