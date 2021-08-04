//
// Created by stubit on 8/4/21.
//

#include "Options.hpp"

Option::Option(const String &text, bool selected) : text(text), selected(selected) {}

MenuEntry::MenuEntry(const String &text, GUITaskFactory taskFactory, bool selected)
        : taskFactory(taskFactory), Option(text, selected) {}
