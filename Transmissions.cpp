//
// Created by stubit on 4/25/20.
//

#include "Transmissions.hpp"

Transmissions::Transmissions(HC12 &module, const String &debugName) : module(module), moduleDebugName(debugName) {}

void Transmissions::poll() {
    while (module.available()) {
        if (durationTransmissionStatus != unfinished) {
            switch (module.read()) {
                case Signal::ping:
                    ++receivedPingCount;
                    break;

                case Signal::pong:
                    pingStatus = finished;
                    pingResponseTime = millis() - pingStart;
                    break;

                case Signal::buzzer:
                    buzzerTime = millis();
                    break;

                case Signal::time:
                    durationTransmissionStatus = unfinished;
                    transmittedDurationBytes = 0;
                    transmittedDuration = 0;
                    durationTransmissionStart = millis();
            }
        } else {
            ++transmittedDurationBytes;

            if(transmittedDurationBytes == 4) {
                durationTransmissionStatus = finished;
                ++receivedDurationCount;
            }

            transmittedDuration |= static_cast<unsigned long>(module.read()) << (8u * (4 - transmittedDurationBytes));
        }
    }

    if(pingStatus == unfinished && millis() - pingStart > pingTimeout) {
        pingStatus = timeout;
    }

    if(millis() - durationTransmissionStart > transmissionTimeout) {
        durationTransmissionStatus = timeout;
    }
}

bool Transmissions::sendPing(unsigned long timeout) {
    if(pingStatus != unfinished) {
        pingStatus = unfinished;
        Transmissions::pingTimeout = timeout;

        pingStart = millis();

        module.write(static_cast<byte>(Signal::ping));

        return true;
    }

    return false;
}

TransmissionStatus Transmissions::getPingStatus() const {
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

void Transmissions::sendPingResponse() const {
    module.write(static_cast<byte>(Signal::pong));
}

void Transmissions::waitOnPing() {
    while (pingStatus == unfinished) {
        poll();
    }
}

void Transmissions::sendBuzzerSignal() {
    module.write(static_cast<byte>(Signal::buzzer));
}

unsigned long Transmissions::getBuzzerReceiveTime() const {
    return buzzerTime;
}

TransmissionStatus Transmissions::getTimeTransmissionStatus() const {
    return durationTransmissionStatus;
}

unsigned long Transmissions::getTransmittedDuration() const {
    return durationTransmissionStatus == finished ? transmittedDuration : 0;
}

void Transmissions::sendDuration(unsigned long duration) const {
    module.write(static_cast<byte>(Signal::time));
    module.write(duration);
}

byte Transmissions::getDurationNumber() const {
    return receivedDurationCount;
}
