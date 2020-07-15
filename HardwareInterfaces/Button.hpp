//
// Created by stubit on 6/19/20.
//

#ifndef SPORTBUZZER_BUTTON_HPP
#define SPORTBUZZER_BUTTON_HPP

#include <Arduino.h>

class Button {
public:
    explicit Button(uint8_t pin);

    bool released();

private:
    const uint8_t pin;
    bool previousState;
};


#endif //SPORTBUZZER_BUTTON_HPP
