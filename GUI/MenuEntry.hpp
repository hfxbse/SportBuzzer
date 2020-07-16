//
// Created by stubit on 7/16/20.
//

#ifndef SPORTBUZZER_MENU_ENTRY_HPP
#define SPORTBUZZER_MENU_ENTRY_HPP

#include <WString.h>
#include "GUITask.hpp"

typedef GUITask * (*GUITaskFactory)();

class MenuEntry {
public:
    explicit MenuEntry(const String &text, GUITaskFactory taskFactory, bool selected = false);

    const String text;
    bool selected;

    GUITaskFactory taskFactory;
};

#endif //SPORTBUZZER_MENU_ENTRY_HPP
