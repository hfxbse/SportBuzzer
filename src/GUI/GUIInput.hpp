//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_GUI_INPUT_HPP
#define SPORTBUZZER_GUI_INPUT_HPP

#include "../HardwareInterfaces/Button.hpp"

class GUIInput {
public:
    void poll();

    bool previous() const;
    bool next() const;
    bool confirm() const;

private:
    static inline Button previousButton = Button(5), nextButton = Button(6), confirmButton = Button(7);
    bool previousState = false, nextState = false, confirmState = false;
};

#endif //SPORTBUZZER_GUI_INPUT_HPP
