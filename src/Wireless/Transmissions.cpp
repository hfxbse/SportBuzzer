//
// Created by stubit on 4/25/20.
//

#include "Transmissions.hpp"

Transmissions::Transmissions(HC12 &module) : module(module) {}

void Transmissions::poll() {
    while (module.available()) {
        if (durationTransmissionStatus != unfinished && limitTransmissionStatus != unfinished) {
            switch (module.read()) {
                case Signal::ping:
                    ++receivedPingCount;
                    break;

                case Signal::pong:
                    pingStatus = finished;
                    pingResponseTime = millis() - pingStart;
                    break;

                case Signal::cancel:
                    ++receivedCancelCount;
                    break;

                case Signal::time:
                    durationTransmissionStatus = unfinished;
                    transmittedDurationBytes = 0;
                    transmittedDuration = 0;
                    durationTransmissionStart = millis();
                    break;

                case Signal::limit:
                    limitTransmissionStatus = unfinished;
                    transmittedLimitBytes = 0;
                    transmittedLimit = 0;
                    limitTransmissionStart = millis();
                    break;

                case Signal::stopwatch:
                    stopwatchTime = millis();
                    break;

                case Signal::timer:
                    timerTime = millis();
                    break;
            }
        } else if (durationTransmissionStatus == unfinished) {
            transmittedDuration = writeData(
                    transmittedDuration,
                    transmittedDurationBytes,
                    durationTransmissionStatus,
                    receivedDurationCount
            );
        } else if (limitTransmissionStatus == unfinished) {
            transmittedLimit = writeData(
                    transmittedLimit,
                    transmittedLimitBytes,
                    limitTransmissionStatus,
                    receivedLimitCount
            );
        }
    }

    if (pingStatus == unfinished && millis() - pingStart > pingTimeout) {
        pingStatus = timeout;
    }

    if (millis() - durationTransmissionStart > transmissionTimeout) {
        durationTransmissionStatus = timeout;
    }

    if (millis() - limitTransmissionStart > transmissionTimeout) {
        limitTransmissionStatus = timeout;
    }
}

bool Transmissions::sendPing(unsigned long timeout) {
    if (pingStatus != unfinished) {
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

unsigned long Transmissions::getPingTime() const {
    return pingStart;
}

unsigned long Transmissions::popReceivedPing() {
    if (receivedPingCount) {
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

void Transmissions::sendStopwatchSignal() const {
    module.write(static_cast<byte>(Signal::stopwatch));
}

unsigned long Transmissions::getStopwatchSignalTime() const {
    return stopwatchTime;
}

void Transmissions::sendTimerSignal() const {
    module.write(static_cast<byte>(Signal::timer));
}

unsigned long Transmissions::getTimerSignalTime() const {
    return timerTime;
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

TransmissionStatus Transmissions::getLimitTransmissionStatus() const {
    return limitTransmissionStatus;
}

unsigned int Transmissions::getTransmittedLimit() const {
    return transmittedLimit;
}

void Transmissions::sendLimit(unsigned int limit) const {
    module.write(static_cast<byte>(Signal::limit));
    module.write(limit);
}

byte Transmissions::getLimitNumber() const {
    return receivedLimitCount;
}

void Transmissions::sendCancelSignal() const {
    module.write(static_cast<byte>(Signal::cancel));
}

byte Transmissions::getCancelNumber() const {
    return receivedCancelCount;
}

template<typename T>
T Transmissions::writeData(T value, byte &transmittedByteCount, TransmissionStatus &status,
                           byte &transmissionNumber) {
    ++transmittedByteCount;

    if (transmittedByteCount == sizeof(T)) {
        status = finished;
        ++transmissionNumber;
    }

    return value | static_cast<T>(module.read()) << (8u * (sizeof(T) - transmittedByteCount));
}
