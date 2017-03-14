#ifndef AIM_INPUT_COMPONENT_H
#define AIM_INPUT_COMPONENT_H

#include "InputComponent.h"

class AimInputComponent : public InputComponent {
public:
    AimInputComponent();

    ~AimInputComponent();

    void pollInput();
    double getChargeTime();

    bool toggleXRotation;
    bool toggleYRotation;
    bool toggleThrow;
    float timePressed;
    float rotationXDirection;
    float rotationYDirection;
    double pressTime;

private:
    void pollGamepad();
    void pollKeyboard();

    bool triggerAvailable;
    bool actionHoldDown;
    double pressStart;

};

#endif
