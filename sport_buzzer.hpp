//
// Created by stubit on 4/15/20.
//

#ifndef SPORTBUZZER_SPORT_BUZZER_HPP
#define SPORTBUZZER_SPORT_BUZZER_HPP

#include "Transmissions.hpp"

void checkHC12Result(bool result, const String &msg);

void testHC12(const String& debugName);

void setupHC12(const String &debugName);

void handlePingSignals(Transmissions &transmissions);

unsigned long getPingDuration(Transmissions &transmissions);

#endif //SPORTBUZZER_SPORT_BUZZER_HPP
