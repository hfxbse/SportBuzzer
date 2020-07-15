//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_BUTTONS_HPP
#define SPORTBUZZER_BUTTONS_HPP

#include "HardwareInterfaces//Button.hpp"

namespace Buttons {
    enum class ButtonType {
        previous = -1,
        next = 1,
        confirm = 0
    };

    extern Button previous, next, confirm;

    template<typename Func>
    void forEachButton(Func callback) {
        callback(previous, ButtonType::previous);
        callback(next, ButtonType::next);
        callback(confirm, ButtonType::confirm);
    }
}

#endif //SPORTBUZZER_BUTTONS_HPP
