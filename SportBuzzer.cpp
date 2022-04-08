#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"

#include "src/HardwareInterfaces/Display.hpp"
#include "src/GUI/StatusBar.hpp"
#include "src/GUI/MainMenu.hpp"
#include "src/Wireless/Connection.hpp"

#ifdef __AVR
#define BUTTON 2
#else

#include <HardwareSerial.h>
#include <variant_generic.h>

HardwareSerial serial(PA10, PA9);
#define Serial serial

#define BUTTON PB3
#endif

#define TIMEOUT 2000
#define VOLTAGE_SAMPLES 20

// 3.2V reference voltage, 10-Bit analog read resolution (values from 0 to 1023)
#define VOLTAGE_TO_READ(voltage) voltage / 3.2f * 1023

volatile unsigned long buzzerTime = 0;

void setup() {
    Serial.begin(9600);

    Connection::hc12.start(B9600);

    pinMode(BUTTON, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(BUTTON), static_cast<void (*)()>([]() {
        if (buzzerTime == 0 || millis() - buzzerTime > 100) {
            buzzerTime = millis();
        }
    }), RISING);

    Connection::testModule();
    Connection::setup();
}

void loop() {
    static GUITask *task = new MainMenu();
    static GUITask *prevTask = nullptr;

    static bool connected = false;
    static uint16_t barHeight = 0;

    static Display display(PA1, PB13, PB12, PA4);

    static Transmissions transmissions(Connection::hc12);
    transmissions.poll();

    // region update battery status
    bool charging = digitalRead(PB8);
    static uint16_t batteryLevel = !charging * 100;

    unsigned long rawVoltageSum = 0;
    for (unsigned i = 0; i < VOLTAGE_SAMPLES; ++i) {
        rawVoltageSum += analogRead(PA6);
    }

    // Charges up to 4.2V, cuts off at 2.4V, voltage divider via a 200k and a 340k resistors to get readable values
    // Battery voltage starts dropping rapidly after 3.2V, before that it barely sinks at all
    // Therefore is the upper bound 2.64V and the lower bound 2.0V, adding a bit of buffer to the upper bound
    const unsigned int battery = constrain(map(
            rawVoltageSum,
            VOLTAGE_TO_READ(2.00f) * VOLTAGE_SAMPLES,
            VOLTAGE_TO_READ(2.55f) * VOLTAGE_SAMPLES,
            0,
            100
    ), 0, 100);

    if ((charging && batteryLevel < battery) || (!charging && batteryLevel > battery)) {
        barHeight = drawStatusBar(display, connected, batteryLevel);
        task->update(display, transmissions, buzzerTime, true, barHeight);
        display.update();

        batteryLevel = battery;
    }
    // endregion

    // region gui task handler
    const bool redraw = task != prevTask;

    if (display.getInstructionCount() == 0) {
        barHeight = drawStatusBar(display, connected, batteryLevel);
    }

    auto newTask = task->update(display, transmissions, buzzerTime, redraw, barHeight);

    static String channel = ChannelSelector::currentChannel();

    if (channel != ChannelSelector::currentChannel()) {
        display.drawRectangle(Display::left(0), Display::top(0), Display::right(0), barHeight, GxEPD_WHITE);
        drawStatusBar(display, connected, batteryLevel);
        display.update();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnusedValue"

        // value is updated used to prevent the screen from refreshing indefinitely
        channel = ChannelSelector::currentChannel();

#pragma clang diagnostic pop
    }

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
