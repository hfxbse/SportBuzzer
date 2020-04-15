#include "HC12.hpp"

#define HC12_1_TX_PIN 10
#define HC12_1_RX_PIN 11
#define HC12_1_SET_PIN 7
#define HC12_1_DEBUG_NAME "HC12_1"

#define HC12_2_TX_PIN 8
#define HC12_2_RX_PIN 9
#define HC12_2_SET_PIN 6
#define HC12_2_DEBUG_NAME "HC12_2"

void checkHC12Result(bool result, const String &msg) {
    if(!result) {
        Serial.println(msg);
    }
}

void testHC12(HC12 &module, const String& debugName) {
    checkHC12Result(module.test(), "Unable to connect to " + debugName);
}

void restoreHC12Defaults(HC12 &module, const String &debugName) {
    checkHC12Result(module.restoreDefaults(), "Unable to restore defaults on " + debugName);
}

void setup() {
    pinMode(HC12_1_SET_PIN, OUTPUT);

    HC12 hc12_1(HC12_1_TX_PIN, HC12_1_RX_PIN, HC12_1_SET_PIN);
    HC12 hc12_2(HC12_2_TX_PIN, HC12_2_RX_PIN, HC12_2_SET_PIN);

    Serial.begin(9600);

    hc12_1.start(B9600);
    testHC12(hc12_1, HC12_1_DEBUG_NAME);
    restoreHC12Defaults(hc12_1, HC12_1_DEBUG_NAME);
    hc12_1.end();

    hc12_2.start(B9600);
    testHC12(hc12_2, HC12_2_DEBUG_NAME);
    restoreHC12Defaults(hc12_2, HC12_2_DEBUG_NAME);
    hc12_2.end();
}

void loop() {}
