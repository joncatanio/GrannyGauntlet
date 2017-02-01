#include "AimInputComponent.h"

#include "GameManager.h"
#include "GameObject.h"
#include "GameWorld.h"

AimInputComponent::AimInputComponent() {
    toggleXRotation = false;
    toggleYRotation = false;
    spacePressed = false;
}

AimInputComponent::~AimInputComponent() {

}

// rotate direction with rotate if pressed

void AimInputComponent::pollInput() {
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        toggleXRotation = true;
        rotationDirection = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        toggleXRotation = true;
        rotationDirection = -1.0f;
    } else {
        toggleXRotation = false;
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        toggleYRotation = true;
        rotationDirection = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        toggleYRotation = true;
        rotationDirection = -1.0f;
    } else {
        toggleYRotation = false;
    }

    toggleThrow = false;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if(!spacePressed){
            spacePressed = true;
            pressStart = glfwGetTime();
        }
        //toggleThrow = true;
    } else {
        if(spacePressed){
            toggleThrow = true;
            pressTime = glfwGetTime() - pressStart;
            spacePressed = false;
        }
    }
}
