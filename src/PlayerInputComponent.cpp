#include "PlayerInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

PlayerInputComponent::PlayerInputComponent() {

}

PlayerInputComponent::~PlayerInputComponent() {

}

void PlayerInputComponent::pollInput() {
   glm::vec3 xDirection, zDirection;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
      holder_->toggleXMovement = true;
      xDirection = glm::vec3(-1.0f, 0.0f, 0.0f);
      holder_->velocity = 12.0f;
	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
      holder_->toggleXMovement = true;
      // Possibly negate the velocity here so the car doesn't turn around.
      xDirection = glm::vec3(1.0f, 0.0f, 0.0f);
      holder_->velocity = 5.0f;
	} else {
      holder_->toggleXMovement = false;
   }

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      holder_->toggleZMovement = true;
      zDirection = glm::vec3(0.0f, 0.0f, 1.0f);
      holder_->velocity = 12.0f;
	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      holder_->toggleZMovement = true;
      zDirection = glm::vec3(0.0f, 0.0f, -1.0f);
      holder_->velocity = 12.0f;
	} else {
      holder_->toggleZMovement = false;
   }

   holder_->direction = xDirection + zDirection;
}
