//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_GUI_INPUT_HPP
#define SPORTBUZZER_GUI_INPUT_HPP

#include "../HardwareInterfaces/Button.hpp"

#ifdef __AVR
#define PREVIOUS_BUTTON 5
#define NEXT_BUTTON 6
#define CONFIRM_BUTTON 7
#else
#define PREVIOUS_BUTTON PB13
#define NEXT_BUTTON PB14
#define CONFIRM_BUTTON PB15
#endif

class GUIInput {
public:
    void poll();

    bool previous() const;

    bool next() const;

    bool confirm() const;

private:
    static inline Button previousButton = Button(PREVIOUS_BUTTON);
    static inline Button nextButton = Button(NEXT_BUTTON);
    static inline Button confirmButton = Button(CONFIRM_BUTTON);
    bool previousState = false, nextState = false, confirmState = false;
};

#endif //SPORTBUZZER_GUI_INPUT_HPP
