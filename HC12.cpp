//
// Created by stubit on 4/13/20.
//

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
    digitalWrite(setPin, LOW);

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

void HC12::write(uint8_t byte) {
    data([this, &byte]() {
        softwareSerial.write(byte);
    });
}

void HC12::write(const char *str) {
    data([this, str]() {
        softwareSerial.write(str);
    });
}

void HC12::write(uint8_t *buffer, size_t size) {
    data([this, buffer, &size]() {
        softwareSerial.write(buffer, size);
    });
}

void HC12::write(const char *buffer, size_t size) {
    data([this, buffer, &size]() {
        softwareSerial.write(buffer, size);
    });
}

void HC12::write(unsigned long value) {
    data([this, &value]() {
        for (auto i = 0; i < sizeof(unsigned long); ++i) {
            softwareSerial.write(value >> (8u * (3 - i)));
        }
    });
}

template<typename Func>
void HC12::data(Func writeCallback) {
    digitalWrite(setPin, HIGH);
    writeCallback();
}

void HC12::start(Baud baudRate) {
    softwareSerial.begin(baudRate);
}

void HC12::end() {
    softwareSerial.end();
}

int HC12::available() {
    int available;

    data([this, &available] {
        available = softwareSerial.available();
    });

    return available;
}

int HC12::read() {
    int read;

    data([this, &read]() {
        read = softwareSerial.read();
    });

    return read;
}

void HC12::flush() {
    softwareSerial.flush();
}

void HC12::print(const char *str) {
    data([this, &str]() {
        softwareSerial.print(str);
    });
}

void HC12::print(const String &string) {
    data([this, &string] {
        softwareSerial.print(string);
    });
}

