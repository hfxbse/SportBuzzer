//
// Created by stubit on 4/13/20.
//

#ifndef SPORTBUZZER_HC12_HPP
#define SPORTBUZZER_HC12_HPP

#include <SoftwareSerial.h>
#include <Arduino.h>
#include "HC12_Options.hpp"

class HC12 {
public:
    HC12(int rxPin, int txPin, int setPin /* requires pull-down resistor */);
    HC12(const HC12 &) = delete;
    HC12(HC12 &&) = delete;

    HC12 &operator=(const HC12 &) = delete;
    HC12 &operator=(HC12 &&) = delete;

    virtual ~HC12();

    void start(Baud baudRate);
    void end();

    bool test();

    bool setTransmissionBaud(Baud baud = B9600);
    bool setChannel(Channel channel = C001);
    bool setTransmissionPower(TransmissionPowerMode powerMode = TransmissionPowerMode::P8);
    bool restoreDefaults();

    void print(const char *str);
    void print(const String &string);

    void write(uint8_t byte);
    void write(const char *str);
    void write(uint8_t *buffer, size_t size);
    void write(const char *buffer, size_t size);
    void write(unsigned long value);

    int available();

    int read();
    void flush();

private:
    const int rxPin, txPin, setPin;

    SoftwareSerial softwareSerial;

    template<typename Func>
    void data(Func writeCallback);

    bool inAtMode = false;
    bool operation(const String &command, unsigned delayDuration = 50);

    bool wasOperationSuccessful();
};


#endif //SPORTBUZZER_HC12_HPP
