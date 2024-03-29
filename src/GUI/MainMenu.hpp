#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_MAIN_MENU_HPP
#define SPORTBUZZER_MAIN_MENU_HPP


#include "GUITask.hpp"
#include "Options.hpp"
#include "ChannelSelector.hpp"
#include "Stopwatch.hpp"
#include "Timer.hpp"
#include <WString.h>

class MainMenu : public GUITask {
public:
    GUITask *update(Display &display, Transmissions &transmissions, unsigned long buzzerTime, bool redraw,
                    uint16_t yOffset) override;

private:
    void draw(Display &display, uint16_t yOffset);

    void previousMenuEntry();

    void nextMenuEntry();

    static const size_t MENU_LENGTH = 3;
    MenuEntry menuEntries[MENU_LENGTH] = {
            MenuEntry(
                    "Stoppuhr",
                    [](Transmissions &transmissions, unsigned long buzzerTime) -> GUITask * {
                        return new Stopwatch(transmissions, buzzerTime);
                    },
                    true
            ),
            MenuEntry(
                    "Timer",
                    [](Transmissions &transmissions, unsigned long buzzerTime) -> GUITask * {
                        return new Timer(transmissions, buzzerTime);
                    }
            ),
            MenuEntry(
                    "Kanalwahl",
                    [](Transmissions &, unsigned long) -> GUITask * {
                        return new ChannelSelector();
                    }
            ),
    };
};

#endif //SPORTBUZZER_MAIN_MENU_HPP

#pragma clang diagnostic pop