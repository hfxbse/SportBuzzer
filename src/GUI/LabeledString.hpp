//
// Created by stubit on 8/5/21.
//

#ifndef SPORTBUZZER_LABELEDSTRING_HPP
#define SPORTBUZZER_LABELEDSTRING_HPP

#include <WString.h>

#define SECOND 1000
#define MINUTE (60 * SECOND)
#define HOUR (60 * MINUTE)
#define DAY (24 * HOUR)

String digits(uint16_t number, size_t min = 2);

uint16_t getAvailableHeight(uint16_t offsets);

uint16_t getMargin(float percentage, uint16_t availableHeight);

uint16_t alignVertically(uint16_t yOffset, uint16_t availableHeight, uint16_t requiredSpace);

String getTimeString(unsigned long duration, bool hidden = false);

#endif //SPORTBUZZER_LABELEDSTRING_HPP
