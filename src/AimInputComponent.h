#ifndef AIM_INPUT_COMPONENT_H
#define AIM_INPUT_COMPONENT_H

#include "InputComponent.h"

class AimInputComponent : public InputComponent {
public:
    AimInputComponent();

    ~AimInputComponent();

    void pollInput();

    bool toggleXRotation;
    bool toggleYRotation;
    bool toggleThrow;
    float timePressed;
    float rotationDirection;
    double pressTime;

private:
    bool spaceHoldDown;
    double pressStart;

};

#endif
