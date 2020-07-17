//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_CONNECTION_HPP
#define SPORTBUZZER_CONNECTION_HPP

#include "Transmissions.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#define TIMEOUT 2000

namespace Connection {
    void checkHC12Result(bool result, const String &msg);

    void testHC12();

    void setupHC12();

    size_t handlePingSignals(Transmissions &transmissions);

    extern HC12 hc12;

    void drawConnectionStatus(TransmissionStatus status);
}

#endif //SPORTBUZZER_CONNECTION_HPP
