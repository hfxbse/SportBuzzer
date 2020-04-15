//
// Created by stubit on 4/15/20.
//

#ifndef SPORTBUZZER_SPORT_BUZZER_HPP
#define SPORTBUZZER_SPORT_BUZZER_HPP

#include <WString.h>
#include "HC12.hpp"

void checkHC12Result(bool result, const String &msg);

void testHC12(HC12 &module, const String& debugName);

void restoreHC12Defaults(HC12 &module, const String &debugName);

#endif //SPORTBUZZER_SPORT_BUZZER_HPP
