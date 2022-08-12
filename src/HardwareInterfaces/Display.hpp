//
// Created by stubit on 8/1/21.
//

#ifndef SPORTBUZZER_DISPLAY_HPP
#define SPORTBUZZER_DISPLAY_HPP

#include <GxEPD2_BW.h>

// #define DISPLAY_PANEL GxEPD2_213_B72     // Waveshare 2.13 inch b/w e-ink display v2
#define DISPLAY_PANEL GxEPD2_213_B74    // Waveshare 2.13 inch b/w e-ink display v3
#define DISPLAY_TOP_OVERSCAN 6
#define DISPLAY_HEIGHT 122
#define DISPLAY_WIDTH 250

enum TextAlign {
    left,
    centered,
    right
};

class Display {
public:
    Display(int busy, int rst, int dc, int cs);

    Display(const Display &) = delete;

    Display(Display &&) = delete;

    Display &operator=(const Display &) = delete;

    Display &operator=(Display &&) = delete;

    ~Display();

    void updateFull();

    void update();

    size_t getInstructionCount();

    void hibernate();

    void setFont(const GFXfont &font);

    void setFontNow(const GFXfont &font);

    void setFontColor(uint16_t color);

    const void getTextBounds(const String &text, uint16_t *width, uint16_t *height);

    void alignText(uint16_t x, uint16_t y, uint16_t width, uint16_t height, TextAlign alignment = TextAlign::left);

    void print(const String &string);

    void drawLine(int16_t offsetX, int16_t offsetY, int16_t endX, int16_t endY, int16_t color = GxEPD_BLACK);

    void drawRectangle(int16_t offsetX, int16_t offsetY, int16_t endX, int16_t endY, uint16_t color = GxEPD_BLACK);

    static int16_t top(float percentage, uint16_t height = DISPLAY_HEIGHT);

    static int16_t bottom(float percentage, uint16_t height = DISPLAY_HEIGHT);

    static int16_t left(float percentage, uint16_t width = DISPLAY_WIDTH);

    static int16_t right(float percentage, uint16_t width = DISPLAY_WIDTH);

private:
    GxEPD2_BW<DISPLAY_PANEL, DISPLAY_PANEL::HEIGHT> display;

    // lambda functions need to capture values by value if they will get destroyed before the display update otherwise
    void addDrawCallback(std::function<void()> drawCallback);

    void adjustUpdateZone(unsigned offsetX, unsigned offsetY, unsigned endX, unsigned endY);

    void draw();

    unsigned offsetX = 0, offsetY = 0, width = 0, height = 0;

    std::function<void()> *drawCallbacks = nullptr;
    size_t drawCallbacksCount = 0, drawCallbacksAllocated = 0;

    bool hibernating = false;
};


#endif //SPORTBUZZER_DISPLAY_HPP
