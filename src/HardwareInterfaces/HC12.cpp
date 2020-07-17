//
// Created by stubit on 4/13/20.
//

#include <USBAPI.h>
#include "HC12.hpp"

HC12::HC12(const int rxPin, const int txPin, const int setPin)
        : rxPin(rxPin), txPin(txPin), setPin(setPin), softwareSerial(SoftwareSerial(rxPin, txPin)) {
    pinMode(setPin, OUTPUT);
}

HC12::~HC12() {
    softwareSerial.end();
}

bool HC12::test() {
    return operation("AT");
}

bool HC12::operation(const String &command, unsigned delayDuration) {
    if(!inAtMode) {
        digitalWrite(setPin, LOW);
        delay(200);

        inAtMode = true;
    }

    flush();
    softwareSerial.print(command);

    delay(delayDuration);

    return wasOperationSuccessful();
}

bool HC12::wasOperationSuccessful() {
    return softwareSerial.available() && softwareSerial.readString().substring(0, 2).equals("OK");
}

bool HC12::setTransmissionBaud(Baud baud) {
    return operation("AT+B" + String(baud));
}

bool HC12::setChannel(Channel channel) {
    String channelString = String(channel);

    while (channelString.length() != 3) {
        channelString = "0" + channelString;
    }

    return operation("AT+C" + channelString);
}

bool HC12::setTransmissionPower(TransmissionPowerMode powerMode) {
    return operation("AT+P" + String(powerMode));
}

bool HC12::restoreDefaults() {
    return operation("AT+DEFAULT", 75);
}

void HC12::dataMode() {
    digitalWrite(setPin, HIGH);
    inAtMode = false;
}

void HC12::start(Baud baudRate) {
    softwareSerial.begin(baudRate);
}

void HC12::end() {
    softwareSerial.end();
}

int HC12::available() {
    dataMode();
    return softwareSerial.available();
}

int HC12::read() {
    dataMode();
    return softwareSerial.read();
}

void HC12::flush() {
    softwareSerial.flush();
}

