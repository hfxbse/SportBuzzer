//
// Created by stubit on 4/25/20.
//

#ifndef SPORTBUZZER_TRANSMISSIONS_HPP
#define SPORTBUZZER_TRANSMISSIONS_HPP


#include "HC12.hpp"

enum Signal {
    ping = 1,
    pong = 2,
};

enum TransmissionStatus {
    unfinished = 1,
    finished = 0,
    timeout = -1,
};

class Transmissions {
public:
    Transmissions(HC12 &module, const String &debugName);

    void poll();

    bool sendPing(unsigned long timeout);
    TransmissionStatus getPingStatus() const;
    unsigned long getPingResponseTime() const;

    unsigned long popReceivedPing();
    void sendPingResponse();

private:
    HC12 &module;
    const String &debugName;

    unsigned long pingStart = 0, pingResponseTime = 0, receivedPingCount = 0, pingTimeout = 0;
    TransmissionStatus pingStatus = finished;
};


#endif //SPORTBUZZER_TRANSMISSIONS_HPP
