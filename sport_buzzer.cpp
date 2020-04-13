#include <Arduino.h>
#include "HC12.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 7

HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);

void setup() {
    Serial.begin(9600);

    if(hc12.test()) {
        Serial.print("hc12 connected");
    } else {
        Serial.print("hc12 connection failed");
    }
}

void loop() {}
