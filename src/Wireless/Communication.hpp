//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_COMMUNICATION_HPP
#define SPORTBUZZER_COMMUNICATION_HPP

#include "Transmissions.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#define TIMEOUT 2000

namespace Communication {
    void checkHC12Result(bool result, const String &msg);

    void testHC12();

    void setupHC12();

    size_t handlePingSignals(Transmissions &transmissions);

    extern HC12 hc12;

    void drawConnectionStatus(TransmissionStatus status);
}

#endif //SPORTBUZZER_COMMUNICATION_HPP
