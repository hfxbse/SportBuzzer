//
// Created by stubit on 8/4/21.
//

#ifndef SPORTBUZZER_OPTIONS_HPP
#define SPORTBUZZER_OPTIONS_HPP

#include <WString.h>
#include "GUITask.hpp"
#include <src/Wireless/Transmissions.hpp>

typedef GUITask *(*GUITaskFactory)(Transmissions &, unsigned long buzzerTime);

class Option {
public:
    explicit Option(const String &text, bool selected = false);

    const String text;
    bool selected;
};

class MenuEntry : public Option {
public:
    MenuEntry(const String &text, GUITaskFactory taskFactory, bool selected = false);

    GUITaskFactory taskFactory;
};

#endif //SPORTBUZZER_OPTIONS_HPP
