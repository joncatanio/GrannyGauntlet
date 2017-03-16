#include "PlayerInputComponent.h"

#include <glm/gtx/rotate_vector.hpp>

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "WindowManager.h"

PlayerInputComponent::PlayerInputComponent() :
   inReverse(false) {
}

PlayerInputComponent::~PlayerInputComponent() {

}

void PlayerInputComponent::pollInput() {
   if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {
      pollGamepad();
   } else {
      pollKeyboard();
   }
}

void PlayerInputComponent::pollGamepad() {
   GameManager& gameManager = GameManager::instance();
   Camera& camera = gameManager.getCamera();
   int count;
   const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

   /* For PS4 and Xbox One controllers the axes count will be 6.
    * Indices [0, 1] refer to the left joysticks x and y axes respectively.
    * Indices [2, 3] refer to the right joysticks x and y axes respectively.
    * Indices [4, 5] refer to the left and right triggers respectively.
    * For our purposes y-joy = x-world & x-joy = z-world
    */
   float xComponent = axes[1], zComponent = -axes[0];

#ifdef _WIN32
   xComponent = -xComponent;
#endif

   // Two driving modes: reverse and drive.
   bool modeChangeAvailable = !holder_->toggleMovement;

   // If the joystick crosses the x-axis change driving mode.
   if (xComponent == -1 || xComponent == 1) {
      modeChangeAvailable = true;
      inReverse = false;
   }

   // Clamp deadspace out so the character doesn't move around.
   if (-0.1f < xComponent && xComponent < 0.1f &&
       -0.1f < zComponent && zComponent < 0.1f) {
      holder_->toggleMovement = false;
      inReverse = false;
   } else {
      holder_->toggleMovement = true;
      // The direction vector in Gamepad space.
      glm::vec3 dirGamepad = glm::vec3(xComponent, 0.0f, zComponent);
      float magnitude = glm::length(dirGamepad);

      // Determine driving mode and set velocity based on mode.
      if ((modeChangeAvailable && xComponent > 0) || inReverse) {
         inReverse = true;
         holder_->velocity = magnitude * -holder_->maxBackwardVel; // Reverse

         camera.calcCamAndPlayerOrient(dirGamepad, true);
      } else {
         inReverse = false;
         holder_->velocity = magnitude * holder_->maxForwardVel; // Drive

         camera.calcCamAndPlayerOrient(dirGamepad, false);
      }
   }
}

void PlayerInputComponent::pollKeyboard() {
   GameManager& gameManager = GameManager::instance();
   Camera& camera = gameManager.getCamera();

   WindowManager& windowManager = WindowManager::instance();

	if (windowManager.isKeyPressed(GLFW_KEY_W) ||
      windowManager.isKeyPressed(GLFW_KEY_UP)) {

      holder_->toggleMovement = true;
      holder_->velocity = 12.0f;

	} else if (windowManager.isKeyPressed(GLFW_KEY_S) ||
      windowManager.isKeyPressed(GLFW_KEY_DOWN)) {

      holder_->toggleMovement = true;
      holder_->velocity = -5.0f;

	} else {
      holder_->toggleMovement = false;
   }

	if (windowManager.isKeyPressed(GLFW_KEY_A) ||
      windowManager.isKeyPressed(GLFW_KEY_LEFT)) {

      camera.changeBeta(-0.5);

	} else if (windowManager.isKeyPressed(GLFW_KEY_D) ||
      windowManager.isKeyPressed(GLFW_KEY_RIGHT)) {

      camera.changeBeta(0.5);
	}
}
