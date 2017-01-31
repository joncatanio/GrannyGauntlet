#include "AimInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

AimInputComponent::AimInputComponent() {
    toggleXRotation = false;
    toggleYRotation = false;
}

AimInputComponent::~AimInputComponent() {

}

// rotate direction with rotate if pressed

void AimInputComponent::pollInput() {
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        toggleXRotation = true;
        velocity = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        toggleXRotation = true;
        velocity = -1.0f;
    } else {
        toggleXRotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        toggleYRotation = true;
        velocity = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        toggleYRotation = true;
        velocity = -1.0f;
    } else {
        toggleYRotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        toggleThrow = true;
    } else {
        toggleThrow = false;
    }
}
