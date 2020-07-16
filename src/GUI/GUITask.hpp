//
// Created by stubit on 7/15/20.
//

#ifndef SPORTBUZZER_GUI_TASK_HPP
#define SPORTBUZZER_GUI_TASK_HPP

class GUITask {
public:
    virtual GUITask* update(bool redraw) = 0;
};

#endif //SPORTBUZZER_GUI_TASK_HPP
