//
// Created by stubit on 6/19/20.
//

#include "Button.hpp"

Button::Button(uint8_t pin) : pin(pin) {
    pinMode(pin, INPUT_PULLUP);
    previousState = digitalRead(pin);
}

bool Button::released() {   // on release rather then on pressed, to avoid bouncing
    bool state = digitalRead(pin);

    if (state != previousState) {
        previousState = state;
        return state == HIGH;
    } else {
        return false;
    }
}
