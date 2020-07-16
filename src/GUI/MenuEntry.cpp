//
// Created by stubit on 7/16/20.
//

#include "MenuEntry.hpp"

MenuEntry::MenuEntry(const String &text, GUITaskFactory taskFactory, bool selected)
        : text(text), taskFactory(taskFactory), selected(selected) {}
