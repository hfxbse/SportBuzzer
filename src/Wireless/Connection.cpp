//
// Created by stubit on 7/15/20.
//

#include "Connection.hpp"

namespace Connection {
    HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);

    void setup() {
        hc12.restoreDefaults();
    }

    size_t handlePingSignals(Transmissions &transmissions) {
        size_t pingCount = 0;

        if (transmissions.popReceivedPing()) {
            transmissions.sendPingResponse();

            ++pingCount;
        }

        return pingCount;
    }

    void updateConnectionStatus(TransmissionStatus status, std::function<void(bool)> draw) {
        static TransmissionStatus previousStatus = TransmissionStatus::unfinished;

        if (status != TransmissionStatus::unfinished && previousStatus != status) {
#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

            // value is updated used to prevent the connection status from refreshing indefinitely
            previousStatus = status;

#pragma clang diagnostic pop

            draw(status == TransmissionStatus::finished);
        }
    }
}