#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"

#include <src/HardwareInterfaces/Display.hpp>
#include <src/GUI/StatusBar.hpp>
#include "src/GUI/MainMenu.hpp"
#include "src/Wireless/Connection.hpp"

#ifdef __AVR
#define BUTTON 2
#else
#define BUTTON PB12
#endif

#define TIMEOUT 2000

volatile unsigned long buzzerTime = 0;

void setup() {
    Serial.begin(9600);

    Connection::hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), static_cast<void (*)()>([]() {
        buzzerTime = millis();
    }), RISING);

    Connection::testModule();
    Connection::setup();
}

void loop() {
    static GUITask *task = new MainMenu();
    static GUITask *prevTask = nullptr;

    static bool connected = false;
    static uint16_t batteryLevel = 0, barHeight = 0;

    // region updateFull battery status
    // TODO
    // endregion

    // STM32duino pin mapping not working for display
    static Display display(1 /* A1 */ , 2 /* A2 */, 3 /* A3 */, 4 /* A4 */);

    static Transmissions transmissions(Connection::hc12);
    transmissions.poll();

    // region gui task handler
    const bool redraw = task != prevTask;

    if (display.getInstructionCount() == 0) {
        barHeight = drawStatusBar(display, connected, batteryLevel);
    }

    auto newTask = task->update(display, transmissions, buzzerTime, redraw, barHeight);

    if (redraw) {
        display.updateFull();
    }

    if (newTask != nullptr) {
        prevTask = task;
        task = newTask;

        if (prevTask != task) {
            delete prevTask;
        }
    } else {
        Serial.println("ERROR: No next task provided after finishing the current task.");
        Serial.println();
    }
    // endregion

    Connection::handlePingSignals(transmissions);

    // region check connection
    if (millis() - transmissions.getPingTime() > TIMEOUT + 100) {
        transmissions.sendPing(TIMEOUT);
    }

    Connection::updateConnectionStatus(transmissions.getPingStatus(), [](bool c) {
        connected = c;

        barHeight = drawStatusBar(display, c, batteryLevel);
        task->update(display, transmissions, buzzerTime, true, barHeight);

        display.update();
    });
    // endregion

    display.hibernate();
}

#pragma clang diagnostic pop
