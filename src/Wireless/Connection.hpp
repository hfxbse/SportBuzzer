//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_CONNECTION_HPP
#define SPORTBUZZER_CONNECTION_HPP

#include "Transmissions.hpp"

#ifdef __AVR
#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#else
#define HC12_TX_PIN PB10
#define HC12_RX_PIN PB11
#define HC12_SET_PIN PB1
#endif

#define TIMEOUT 2000

namespace Connection {
    void setup();

    size_t handlePingSignals(Transmissions &transmissions);

    extern HC12 hc12;

    void updateConnectionStatus(TransmissionStatus status, std::function<void(bool connected)> draw);
}

#endif //SPORTBUZZER_CONNECTION_HPP
