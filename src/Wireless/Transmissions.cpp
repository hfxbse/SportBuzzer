//
// Created by stubit on 4/25/20.
//

#include "Transmissions.hpp"

Transmissions::Transmissions(HC12 &module) : module(module) {}

void Transmissions::poll() {
    while (module.available()) {
        if (durationTransmissionStatus != unfinished && limitTransmissionStatus != unfinished) {
            byte signal = module.read();

            switch (signal) {
                case Signal::ping:
                    ++receivedPingCount;
                    break;

                case Signal::pong:
                    pingStatus = finished;
                    pingResponseTime = millis() - pingStart;
                    break;

                case Signal::cancel_stopwatch:
                case Signal::cancel_timer:
                    ++receivedCancelCount;

                    cancelSignal = static_cast<Signal>(signal);
                    break;

                case Signal::duration_stopwatch:
                case Signal::duration_timer:
                    durationTransmissionStatus = unfinished;
                    transmittedDurationBytes = 0;
                    transmittedDuration = 0;
                    durationTransmissionStart = millis();

                    timeSignal = static_cast<Signal>(signal);
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

void Transmissions::sendDuration(unsigned long duration, bool stopwatch) const {
    module.write(static_cast<byte>(stopwatch ? Signal::duration_stopwatch : Signal::duration_timer));
    module.write(duration);
}

byte Transmissions::getDurationNumber() const {
    return receivedDurationCount;
}

Signal Transmissions::getDurationSignal() const {
    return timeSignal;
}

TransmissionStatus Transmissions::getLimitTransmissionStatus() const {
    return limitTransmissionStatus;
}

long Transmissions::getTransmittedLimit() const {
    return transmittedLimit;
}

void Transmissions::sendLimit(long limit) const {
    module.write(static_cast<byte>(Signal::limit));
    module.write(limit);
}

byte Transmissions::getLimitNumber() const {
    return receivedLimitCount;
}

void Transmissions::sendCancelSignal(bool stopwatch) const {
    module.write(static_cast<byte>(stopwatch ? Signal::cancel_stopwatch : Signal::cancel_timer));
}

byte Transmissions::getCancelNumber() const {
    return receivedCancelCount;
}

Signal Transmissions::getCancelSignal() const {
    return cancelSignal;
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
