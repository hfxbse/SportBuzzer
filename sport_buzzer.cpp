#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"

#include "Transmissions.hpp"
#include "Button.hpp"

#define HC12_TX_PIN 10
#define HC12_RX_PIN 11
#define HC12_SET_PIN 9
#define HC12_DEBUG_NAME String("HC12")

#define BUTTON 2
#define TIMEOUT 10000

namespace Buttons {
    enum class ButtonType {
        previous = -1,
        next = 1,
        confirm = 0
    };

    Button previous(5), next(6), confirm(7);

    template<typename Func>
    void forEachButton(Func callback) {
        callback(previous, ButtonType::previous);
        callback(next, ButtonType::next);
        callback(confirm, ButtonType::confirm);
    }
}

namespace Communication {
    void checkHC12Result(bool result, const String &msg);

    void testHC12(const String &debugName);

    void setupHC12(const String &debugName);

    void handlePingSignals(Transmissions &transmissions);

    unsigned long getPingDuration(Transmissions &transmissions);

    HC12 hc12(HC12_TX_PIN, HC12_RX_PIN, HC12_SET_PIN);
}

typedef void *(*(*GUITask)(bool))(bool);

volatile unsigned long timerStart = 0;

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

GUITask channelSelector();

void setup() {
    Serial.begin(9600);

    Communication::hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), []() {
        timerStart = millis();
    }, RISING);

    Communication::testHC12(HC12_DEBUG_NAME);
    Communication::setupHC12(HC12_DEBUG_NAME);
}

void loop() {
    static auto task = reinterpret_cast<GUITask>(MainMenu::mainMenu);
    static GUITask prevTask = nullptr;

    // region update battery status
    // TODO
    // endregion

    // region gui task handler
    auto newTask = reinterpret_cast<GUITask>(task(task != prevTask));
    if (newTask != nullptr) {
        prevTask = task;
        task = newTask;
    } else {
        Serial.println("ERROR: No next task provided after finishing the current task.");
        Serial.println();
    }
    // endregion

    static Transmissions transmissions(Communication::hc12, HC12_DEBUG_NAME);
    static unsigned long oldTimerStart = timerStart;

    transmissions.poll();
    Communication::handlePingSignals(transmissions);

    // region buzzer press handler
    static unsigned long oldReceiveTime = transmissions.getBuzzerReceiveTime();
    const unsigned long receiveTime = transmissions.getBuzzerReceiveTime();

    if (oldTimerStart != timerStart) {
        oldTimerStart = timerStart;
        transmissions.sendBuzzerSignal();
    }

    if (oldReceiveTime != receiveTime) {
        oldReceiveTime = receiveTime;
        unsigned long duration = receiveTime - timerStart - Communication::getPingDuration(transmissions);

        Serial.println("Duration: " + String(duration) + "ms.");
        transmissions.sendDuration(duration);
    }
    // endregion

    // region received duration
    static unsigned long oldDurationNumber = transmissions.getDurationNumber();
    const unsigned long durationNumber = transmissions.getDurationNumber();

    if (oldDurationNumber != durationNumber) {
        oldDurationNumber = durationNumber;
        Serial.println("Received duration: " + String(transmissions.getTransmittedDuration()) + "ms.");
    }
    // endregion
}

namespace MainMenu {
    void drawMenu(const MenuEntry menuEntries[], size_t size) {
        Serial.println("Menu");

        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                Serial.print("x  ");
            } else {
                Serial.print("   ");
            }

            Serial.println(menuEntries[i].text);
        }

        Serial.println();
    }

    void previousMenuEntry(MenuEntry *menuEntries, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                menuEntries[i].selected = false;

                if (i == 0) {
                    menuEntries[size - 1].selected = true;
                } else {
                    menuEntries[i - 1].selected = true;
                }

                return;
            }
        }
    }

    void nextMenuEntry(MenuEntry *menuEntries, size_t size) {
        for (int i = 0; i < size; ++i) {
            if (menuEntries[i].selected) {
                menuEntries[i].selected = false;

                if (i + 1 == size) {
                    menuEntries[0].selected = true;
                } else {
                    menuEntries[i + 1].selected = true;
                }

                return;
            }
        }
    }

    GUITask mainMenu(bool redraw) {
        const size_t menuLength = 3;
        static MenuEntry menuEntries[] = {
                MenuEntry("Entry 1", nullptr, true),
                MenuEntry("Entry 2", nullptr),
                MenuEntry("Channel selection", reinterpret_cast<GUITask>(channelSelector)),
        };

        auto nextTask = reinterpret_cast<GUITask>(mainMenu);

        if (redraw) {
            // region reset selection
            for (int i = 1; i < menuLength; ++i) {
                menuEntries[i].selected = false;
            }

            menuEntries[0].selected = true;
            // endregion

            drawMenu(menuEntries, menuLength);
        } else {
            // region gui button handling
            Buttons::forEachButton([&](Button &button, Buttons::ButtonType type) {
                if (button.released()) {
                    switch (type) {
                        case Buttons::ButtonType::previous:
                            Serial.print("\"Previous\" button");
                            Serial.println(" released.");
                            previousMenuEntry(menuEntries, menuLength);
                            drawMenu(menuEntries, menuLength);
                            break;

                        case Buttons::ButtonType::next:
                            Serial.print("\"Next\" button");
                            Serial.println(" released.");
                            nextMenuEntry(menuEntries, menuLength);
                            drawMenu(menuEntries, menuLength);
                            break;

                        case Buttons::ButtonType::confirm:
                            Serial.print("\"Confirm\" button");
                            Serial.println(" released.");

                            for (auto &menuEntry : menuEntries) {
                                if (menuEntry.selected) {
                                    nextTask = menuEntry.task;
                                    Serial.println();
                                    return;
                                }
                            }

                            Serial.println("ERROR: No menu entry selected.");
                            Serial.println();
                            break;
                    }
                }
            });
            // endregion
        }

        return nextTask;
    }
}

GUITask channelSelector() {
    Serial.println("Channel selector");
    Serial.println();

    return reinterpret_cast<GUITask>(MainMenu::mainMenu);
}

namespace Communication {
    void checkHC12Result(bool result, const String &msg) {
        if (!result) {
            Serial.println(msg);
        }
    }

    void testHC12(const String &debugName) {
        Communication::checkHC12Result(hc12.test(), "Unable to connect to " + debugName + ".");
    }

    void setupHC12(const String &debugName) {
        checkHC12Result(hc12.restoreDefaults(), "Unable to restore defaults on " + debugName + ".");

        Serial.println("Setup completed on " + debugName);
    }

    void handlePingSignals(Transmissions &transmissions) {
        if (transmissions.popReceivedPing()) {
            transmissions.sendPingResponse();
        }
    }

    unsigned long getPingDuration(Transmissions &transmissions) {
        if (transmissions.sendPing(TIMEOUT)) {
            Serial.println("Send ping on " + HC12_DEBUG_NAME + ".");
        }

        transmissions.waitOnPing();
        TransmissionStatus pingStatus = transmissions.getPingStatus();

        if (pingStatus == finished) {
            Serial.println(
                    "Ping on " + HC12_DEBUG_NAME + " took " + String(transmissions.getPingResponseTime()) + "ms."
            );
        } else if (pingStatus == timeout) {
            Serial.println("Ping timeout on " + HC12_DEBUG_NAME + ".");
        }

        return transmissions.getPingResponseTime();
    }
}

#pragma clang diagnostic pop
