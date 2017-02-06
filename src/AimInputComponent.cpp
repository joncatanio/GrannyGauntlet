#include "AimInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

AimInputComponent::AimInputComponent() {
    toggleXRotation = false;
    toggleYRotation = false;
    toggleThrow = false;
    triggerAvailable = true;
    actionHoldDown = false;
}

AimInputComponent::~AimInputComponent() {

}

void AimInputComponent::pollInput() {
   if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
      pollGamepad();
   } else {
      pollKeyboard();
   }
}

void AimInputComponent::pollGamepad() {
   int count;
   const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
   float xComponent = -axes[3], yComponent = -axes[2];
   // A trigger at rest has a value of -1.0, fully engaged is 1.0.
   float leftTrigger = axes[4], rightTrigger = axes[5];

   if (-0.1f < xComponent && xComponent < 0.1f) {
      toggleXRotation = false;
   } else {
      toggleXRotation = true;

      // Slightly faster rotation for the gamepad.
      rotationXDirection = 2.5 * xComponent;
   }
   if (-0.1f < yComponent && yComponent < 0.1f) {
      toggleYRotation = false;
   } else {
      toggleYRotation = true;

      rotationYDirection = 2.5 * yComponent;
   }

   // Restrict cookie throwing to a trigger engage basis, no holding.
   if (triggerAvailable && rightTrigger == 1) {

      triggerAvailable = false;
      if(!actionHoldDown){
         actionHoldDown = true;
         pressStart = glfwGetTime();
      }

   } else if (rightTrigger == -1) {
      triggerAvailable = true;
      toggleThrow = false;

      if (actionHoldDown) {
         toggleThrow = true;
         pressTime = glfwGetTime() - pressStart;
         actionHoldDown = false;
      }
   }
}

// rotate direction with rotate if pressed
void AimInputComponent::pollKeyboard() {
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        toggleXRotation = true;
        rotationXDirection = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        toggleXRotation = true;
        rotationXDirection = -1.0f;
    } else {
        toggleXRotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        toggleYRotation = true;
        rotationYDirection = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        toggleYRotation = true;
        rotationYDirection = -1.0f;
    } else {
        toggleYRotation = false;
    }

    toggleThrow = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if(!actionHoldDown){
            actionHoldDown = true;
            pressStart = glfwGetTime();
        }
        //toggleThrow = true;
    } else {
        if(actionHoldDown){
            toggleThrow = true;
            pressTime = glfwGetTime() - pressStart;
            actionHoldDown = false;
        }
    }
}
