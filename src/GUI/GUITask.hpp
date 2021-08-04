//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_GUI_TASK_HPP
#define SPORTBUZZER_GUI_TASK_HPP

#include <src/HardwareInterfaces/Display.hpp>
#include <src/Wireless/Transmissions.hpp>

class GUITask {
public:
    virtual GUITask *
    update(Display &display, Transmissions &transmissions, unsigned long buzzerTime, bool redraw, uint16_t yOffset) = 0;
};

#endif //SPORTBUZZER_GUI_TASK_HPP
