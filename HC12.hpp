//
// Created by stubit on 4/13/20.
//

#ifndef SPORTBUZZER_HC12_HPP
#define SPORTBUZZER_HC12_HPP

#include <SoftwareSerial.h>
#include "HC12_Options.hpp"

class HC12 {
public:
    HC12(int rxPin, int txPin, int setPin);

    virtual ~HC12();

    void start(Baud baudRate);
    void end();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

    bool test();

    bool setTransmissionBaud(Baud baud = B9600);
    bool setChannel(Channel channel = C001);
    bool setTransmissionPower(TransmissionPowerMode powerMode);
    bool restoreDefaults();

    void write(uint8_t byte);
    void write(const char *str);
    void write(uint8_t *buffer, size_t size);
    void write(const char *buffer, size_t size);

    int available();
    int read();

#pragma clang diagnostic pop

private:
    const int rxPin, txPin, setPin;

    SoftwareSerial softwareSerial;

    template<typename Func>
    void data(Func writeCallback);

    bool operation(const String &command);

    bool wasOperationSuccessful();
};


#endif //SPORTBUZZER_HC12_HPP
