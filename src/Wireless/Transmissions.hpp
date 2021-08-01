//
// Created by stubit on 4/25/20.
//

#ifndef SPORTBUZZER_TRANSMISSIONS_HPP
#define SPORTBUZZER_TRANSMISSIONS_HPP


#include <Arduino.h>
#include "../HardwareInterfaces/HC12.hpp"

enum Signal {
    undefined = 0,
    ping = 1,
    pong = 2,
    cancel_stopwatch = 3,
    cancel_timer = 4,
    duration_stopwatch = 5,
    duration_timer = 6,
    limit = 7,
    stopwatch = 8,
    timer = 9,
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

    TransmissionStatus getDurationTransmissionStatus() const;

    unsigned long getTransmittedDuration() const;

    void sendDuration(unsigned long duration, bool stopwatch) const;

    byte getDurationNumber() const;

    Signal getDurationSignal() const;

    TransmissionStatus getLimitTransmissionStatus() const;

    long getTransmittedLimit() const;

    void sendLimit(long limit) const;

    byte getLimitNumber() const;

    void sendCancelSignal(bool stopwatch) const;

    byte getCancelNumber() const;

    Signal getCancelSignal() const;

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
    Signal cancelSignal = undefined;

    // related to duration transmission
    TransmissionStatus durationTransmissionStatus = finished;
    byte transmittedDurationBytes = 0, receivedDurationCount = 0;
    unsigned long transmittedDuration = 0, durationTransmissionStart = 0;
    Signal timeSignal = Signal::undefined;

    // related to limit transmission
    TransmissionStatus limitTransmissionStatus = finished;
    byte transmittedLimitBytes = 0, receivedLimitCount = 0;
    long transmittedLimit = 0;
    unsigned long limitTransmissionStart = 0;

    template<typename T>
    T writeData(T value, byte &transmittedByteCount, TransmissionStatus &status, byte &transmissionNumber);
};


#endif //SPORTBUZZER_TRANSMISSIONS_HPP
