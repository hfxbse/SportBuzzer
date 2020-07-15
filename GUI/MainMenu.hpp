#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_MAIN_MENU_HPP
#define SPORTBUZZER_MAIN_MENU_HPP


#include "GUITask.hpp"
#include <WString.h>

namespace MainMenu {
    class MenuEntry {
    public:
        explicit MenuEntry(const String &text, GUITask task, bool selected = false)
                : text(text), task(task), selected(selected) {};

        String text;
        bool selected;

        GUITask task;
    };

    void drawMenu(const MenuEntry menuEntries[], size_t size);

    void previousMenuEntry(MenuEntry *menuEntries, size_t size);

    void nextMenuEntry(MenuEntry *menuEntries, size_t size);

    GUITask mainMenu(bool redraw);
}

#endif //SPORTBUZZER_MAIN_MENU_HPP

#pragma clang diagnostic pop