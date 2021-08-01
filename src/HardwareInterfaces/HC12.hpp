//
// Created by stubit on 4/13/20.
//

#ifndef SPORTBUZZER_HC12_HPP
#define SPORTBUZZER_HC12_HPP

#include <SoftwareSerial.h>
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

    template<typename T>
    void write(const T &value) {
        dataMode();

        for (auto i = 0; i < sizeof(T); ++i) {
            softwareSerial.write(value >> (8 * ((sizeof(T) - 1) - i)));
        }
    }

    int available();

    int read();

    void flush();

private:
    const int setPin;

    SoftwareSerial softwareSerial;

    void dataMode();

    bool inAtMode = false;
    bool operation(const String &command, unsigned delayDuration = 50);

    bool wasOperationSuccessful();
};


#endif //SPORTBUZZER_HC12_HPP
