//
// Created by stubit on 8/4/21.
//

#ifndef SPORTBUZZER_NAVIGATIONBAR_HPP
#define SPORTBUZZER_NAVIGATIONBAR_HPP

#include <src/HardwareInterfaces/Display.hpp>
#include "Options.hpp"
#include "Roboto_Medium8pt7b.h"

int16_t drawNavigationBar(Display &display, const Option *options, size_t optionCount);

#endif //SPORTBUZZER_NAVIGATIONBAR_HPP
