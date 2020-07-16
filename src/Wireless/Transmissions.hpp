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
    buzzer = 3,
    time = 4,
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

    void sendBuzzerSignal();
    unsigned long getBuzzerReceiveTime() const;

    unsigned long transmissionTimeout = 10000;

    TransmissionStatus getTimeTransmissionStatus() const;
    unsigned long getTransmittedDuration() const;

    void sendDuration(unsigned long duration) const;

    byte getDurationNumber() const;

private:
    // related to module
    HC12 &module;

    // related to ping signals
    unsigned long pingStart = 0, pingResponseTime = 0, receivedPingCount = 0, pingTimeout = 0;
    TransmissionStatus pingStatus = finished;

    // related to buzzer signal
    unsigned long buzzerTime = 0;

    // related to duration transmission
    TransmissionStatus durationTransmissionStatus = finished;
    byte transmittedDurationBytes = 0, receivedDurationCount = 0;
    unsigned long transmittedDuration = 0, durationTransmissionStart = 0;
};


#endif //SPORTBUZZER_TRANSMISSIONS_HPP
