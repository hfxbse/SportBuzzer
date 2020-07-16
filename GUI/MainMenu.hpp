#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_MAIN_MENU_HPP
#define SPORTBUZZER_MAIN_MENU_HPP


#include "GUITask.hpp"
#include "MenuEntry.hpp"
#include "ChannelSelector.hpp"
#include <WString.h>

class MainMenu : public GUITask {
public:
    GUITask * update(bool redraw);

private:
    void draw();

    void previousMenuEntry();
    void nextMenuEntry();

    static const size_t MENU_LENGTH = 3;
    MenuEntry menuEntries[MENU_LENGTH] = {
            MenuEntry("Entry 1", []() -> GUITask * {return nullptr;}, true),
            MenuEntry("Entry 2", []() -> GUITask * {return nullptr;}),
            MenuEntry("Channel selection", []() -> GUITask * {return new ChannelSelector();}),
    };
};

#endif //SPORTBUZZER_MAIN_MENU_HPP

#pragma clang diagnostic pop