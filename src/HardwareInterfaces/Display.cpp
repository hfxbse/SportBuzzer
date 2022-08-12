//
// Created by stubit on 8/1/21.
//


#include "Display.hpp"

template<typename T>
static void startLessThanEnd(T &start, T &end) {
    if (start > end) {
        T temp = start;
        start = end;
        end = temp;
    }
}

Display::Display(int busy, int rst, int dc, int cs)
        : display(DISPLAY_PANEL(cs, dc, rst, busy)) {
    display.init();
    display.setRotation(1);
}

Display::~Display() {
    display.powerOff();

    if (drawCallbacks) {
        delete[] drawCallbacks;
    }
}

void Display::updateFull() {
    display.setFullWindow();
    draw();
}

void Display::update() {
    display.setPartialWindow(offsetX, offsetY, width, height);
    draw();
}

size_t Display::getInstructionCount() {
    return drawCallbacksCount;
}

void Display::hibernate() {
    if (!hibernating) {
        display.hibernate();
        hibernating = true;
    }
}

void Display::setFont(const GFXfont &font) {
    setFontNow(font);
    addDrawCallback([this, font]() {
        display.setFont(&font);
    });
}

void Display::setFontNow(const GFXfont &font) {
    display.setFont(&font);
}

void Display::setFontColor(uint16_t color) {
    addDrawCallback([this, color]() {
        display.setTextColor(color);
    });
}

const void Display::getTextBounds(const String &text, uint16_t *width, uint16_t *height) {
    display.getTextBounds(text, 0, 0, nullptr, nullptr, width, height);
}

void Display::alignText(
        uint16_t x,
        uint16_t y,
        uint16_t width,
        uint16_t height,
        TextAlign alignment
) {
    switch (alignment) {
        case TextAlign::centered:
            x -= ((static_cast<float>(width) / 2) + 0.5);
            break;
        case TextAlign::right:
            x -= width;
            break;
    }

    adjustUpdateZone(x, y, x + width, y + height);
    addDrawCallback([this, x, y, width, height]() {
        display.setCursor(x, y + height);
    });
}

void Display::print(const String &string) {
    addDrawCallback([this, string]() {
        display.print(string);
    });
}

void Display::drawLine(int16_t offsetX, int16_t offsetY, int16_t endX, int16_t endY, int16_t color) {
    adjustUpdateZone(offsetX, offsetY, endX, endY);

    addDrawCallback([this, offsetX, offsetY, endX, endY, color]() {
        display.drawLine(offsetX, offsetY, endX, endY, color);
    });

}

void Display::drawRectangle(int16_t offsetX, int16_t offsetY, int16_t endX, int16_t endY, uint16_t color) {
    adjustUpdateZone(static_cast<uint16_t>(offsetX), static_cast<uint16_t>(offsetY), endX, endY);

    addDrawCallback([this, offsetX, offsetY, endX, endY, color]() {
        display.fillRect(offsetX, offsetY, endX - offsetX + 1, endY - offsetY + 1, color);
    });
}

int16_t Display::top(float percentage, uint16_t height) {
    return DISPLAY_TOP_OVERSCAN + (percentage / 100) * (height - 1) + 0.5;
}

int16_t Display::bottom(float percentage, uint16_t height) {
    return top(100 - percentage, height);
}

int16_t Display::left(float percentage, uint16_t width) {
    return (percentage / 100) * (width - 1) + 0.5;
}

int16_t Display::right(float percentage, uint16_t width) {
    return left(100 - percentage, width);
}

void Display::addDrawCallback(std::function<void()> drawCallback) {
    ++drawCallbacksCount;

    if (drawCallbacks == nullptr || drawCallbacksCount > drawCallbacksAllocated) {
        std::function<void()> *newAllocation = new std::function<void()>[drawCallbacksCount * 2];

        if (drawCallbacks != nullptr) {

            // cannot use memcpy as it leads to undefined behavior copying the objects
            for (size_t i = 0; i < drawCallbacksAllocated; ++i) {
                newAllocation[i] = drawCallbacks[i];
            }

            delete[] drawCallbacks;
        }

        drawCallbacksAllocated = drawCallbacksCount * 2;

        drawCallbacks = newAllocation;
    }

    drawCallbacks[drawCallbacksCount - 1] = drawCallback;
}

void Display::adjustUpdateZone(unsigned int offsetX, unsigned int offsetY, unsigned int endX, unsigned int endY) {
    startLessThanEnd(offsetX, endX);
    startLessThanEnd(offsetY, endY);

    if (Display::offsetX > offsetX) {
        Display::offsetX = offsetX;
    }

    if (Display::offsetY > offsetY) {
        Display::offsetY = offsetY;
    }

    const unsigned width = (endX - Display::offsetX) + 1;
    if (Display::width < width) {
        Display::width = width;
    }

    const unsigned height = (endY - Display::offsetY) + 1;
    if (Display::height < height) {
        Display::height = height;
    }
}

void Display::draw() {
    hibernating = false;

    display.firstPage();
    display.setTextColor(GxEPD_BLACK);

    do {
        display.fillScreen(GxEPD_WHITE);

        if (drawCallbacks != nullptr) {
            for (int i = 0; i < drawCallbacksCount; ++i) {
                drawCallbacks[i]();
            }
        }

    } while (display.nextPage());

    display.setTextColor(GxEPD_BLACK);

    delete[] drawCallbacks;
    drawCallbacks = nullptr;

    drawCallbacksAllocated = 0;
    drawCallbacksCount = 0;
}

