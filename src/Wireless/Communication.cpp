//
// Created by stubit on 7/15/20.
//

#include "Communication.hpp"

namespace Communication {
    HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);

    void checkHC12Result(bool result, const String &msg) {
        if (!result) {
            Serial.println(msg);
        }
    }

    void testHC12() {
        Communication::checkHC12Result(hc12.test(), "Unable to connect to HC12.");
    }

    void setupHC12() {
        checkHC12Result(hc12.restoreDefaults(), "Unable to restore defaults on HC12.");

        Serial.println("Setup completed on HC12.");
    }

    size_t handlePingSignals(Transmissions &transmissions) {
        size_t pingCount = 0;

        if (transmissions.popReceivedPing()) {
            transmissions.sendPingResponse();

            ++pingCount;
        }

        return pingCount;
    }

    void drawConnectionStatus(TransmissionStatus status) {
        static TransmissionStatus previousStatus = TransmissionStatus::unfinished;

        if (status != TransmissionStatus::unfinished && previousStatus != status) {
            previousStatus = status;

            if (status == TransmissionStatus::finished) {
                Serial.println("Connected");
            } else {
                Serial.println("Not connected");
            }

            Serial.println();
        }
    }
}