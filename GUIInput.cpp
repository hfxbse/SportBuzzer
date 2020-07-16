//
// Created by stubit on 7/15/20.
//

#include "GUIInput.hpp"

void GUIInput::poll() {
    previousState = previousButton.released();
    nextState = nextButton.released();
    confirmState = confirmButton.released();
}

bool GUIInput::previous() const {
    return previousState;
}

bool GUIInput::next() const {
    return nextState;
}

bool GUIInput::confirm() const {
    return confirmState;
}
