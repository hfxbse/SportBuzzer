//
// Created by stubit on 4/25/20.
//

#ifndef SPORTBUZZER_TRANSMISSIONS_HPP
#define SPORTBUZZER_TRANSMISSIONS_HPP


#include <USBAPI.h>
#include "../HardwareInterfaces/HC12.hpp"

enum Signal {
    ping = 1,
    pong = 2,
    cancel = 3,
    time = 4,
    limit = 5,
    stopwatch = 6,
    timer = 7,
};

enum TransmissionStatus {
    unfinished = 1,
    finished = 0,
    timeout = -1,
};

class Transmissions {
public:
    explicit Transmissions(HC12 &module);

    void poll();

    bool sendPing(unsigned long timeout);

    TransmissionStatus getPingStatus() const;

    unsigned long getPingResponseTime();

    unsigned long getPingTime() const;

    unsigned long popReceivedPing();

    void sendPingResponse() const;

    void waitOnPing();

    void sendStopwatchSignal() const;

    unsigned long getStopwatchSignalTime() const;

    void sendTimerSignal() const;

    unsigned long getTimerSignalTime() const;

    unsigned long transmissionTimeout = 5000;

    TransmissionStatus getTimeTransmissionStatus() const;

    unsigned long getTransmittedDuration() const;

    void sendDuration(unsigned long duration) const;

    byte getDurationNumber() const;

    TransmissionStatus getLimitTransmissionStatus() const;

    unsigned int getTransmittedLimit() const;

    void sendLimit(unsigned int limit) const;

    byte getLimitNumber() const;

    void sendCancelSignal() const;

    byte getCancelNumber() const;

private:
    // related to module
    HC12 &module;

    // related to ping signals
    unsigned long pingStart = 0, pingResponseTime = 0, receivedPingCount = 0, pingTimeout = 0;
    TransmissionStatus pingStatus = finished;

    // related to stopwatch signal
    unsigned long stopwatchTime = 0;

    // related to timer signal
    unsigned long timerTime = 0;

    // related to cancel signal
    byte receivedCancelCount = 0;

    // related to duration transmission
    TransmissionStatus durationTransmissionStatus = finished;
    byte transmittedDurationBytes = 0, receivedDurationCount = 0;
    unsigned long transmittedDuration = 0, durationTransmissionStart = 0;

    // related to limit transmission
    TransmissionStatus limitTransmissionStatus = finished;
    byte transmittedLimitBytes = 0, receivedLimitCount = 0;
    unsigned int transmittedLimit = 0;
    unsigned long limitTransmissionStart = 0;

    template<typename T>
    T writeData(T value, byte &transmittedByteCount, TransmissionStatus &status, byte &transmissionNumber);
};


#endif //SPORTBUZZER_TRANSMISSIONS_HPP
