#include "PlayerInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"
#include <glm/gtx/rotate_vector.hpp>

PlayerInputComponent::PlayerInputComponent() {

}

PlayerInputComponent::~PlayerInputComponent() {

}

void PlayerInputComponent::pollInput() {
   GameManager& gameManager = GameManager::instance();
   Camera& camera = gameManager.getCamera();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {

      holder_->toggleMovement = true;
      holder_->velocity = 12.0f;

	} else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {

      holder_->toggleMovement = true;
      holder_->velocity = -5.0f;

	} else {
      holder_->toggleMovement = false;
   }

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

      camera.changeBeta(-0.5);

	} else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {

      camera.changeBeta(0.5);

	}
}
