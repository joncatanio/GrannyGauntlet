#include "AimInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "WindowManager.h"

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
   float rightTrigger = axes[5];

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

   // Restrict cookie throwing to single and charged shot, no automatic mode.
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
    WindowManager& windowManager = WindowManager::instance();

    if (windowManager.isKeyPressed(GLFW_KEY_I)) {
        toggleXRotation = true;
        rotationXDirection = 1.0f;
    } else if (windowManager.isKeyPressed(GLFW_KEY_K)) {
        toggleXRotation = true;
        rotationXDirection = -1.0f;
    } else {
        toggleXRotation = false;
    }

    if (windowManager.isKeyPressed(GLFW_KEY_J)) {
        toggleYRotation = true;
        rotationYDirection = 1.0f;
    } else if (windowManager.isKeyPressed(GLFW_KEY_L)) {
        toggleYRotation = true;
        rotationYDirection = -1.0f;
    } else {
        toggleYRotation = false;
    }

    toggleThrow = false;
    if (windowManager.isKeyPressed(GLFW_KEY_SPACE)) {
        if(!actionHoldDown){
            actionHoldDown = true;
            pressStart = glfwGetTime();
        }
    } else {
        if(actionHoldDown){
            toggleThrow = true;
            pressTime = glfwGetTime() - pressStart;
            actionHoldDown = false;
        }
    }
}
