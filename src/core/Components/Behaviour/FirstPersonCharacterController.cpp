#include "FirstPersonCharacterController.h"

#include <iostream>
#include <vec2.hpp>
#include <GLFW/glfw3.h>

#include "core/Application.h"
#include "core/GameObject.h"
#include "core/GameTime.h"
#include "core/Components/Camera/Camera.h"

bool mouseEnabled = false;
bool pressedF11 = false;

void FirstPersonCharacterController::start() {
    glfwGetCursorPos(Application::GetCurrentWindow(), &lastX, &lastY);
}

void FirstPersonCharacterController::update() {

    int F11 = glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_F11);

    if (!pressedF11 && F11 == GLFW_PRESS) {
        if (!mouseEnabled) {
            pressedF11 = true;
            mouseEnabled = true;
        } else {
            pressedF11 = true;
            mouseEnabled = false;
        }
    }

    if (pressedF11 && F11 == GLFW_RELEASE) {
        pressedF11 = false;
    }

    if (mouseEnabled) {
        glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }
    glfwSetInputMode(Application::GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float velocity = speed * GameTime::DeltaTime;

    int W = glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_W);
    int S = glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_S);
    int A = glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_A);
    int D = glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_D);

    if (W == GLFW_PRESS) {
        gameObject->m_position += gameObject->Forward() * velocity;
    }

    if (S == GLFW_PRESS) {
        gameObject->m_position -= gameObject->Forward() * velocity;
    }

    if (A == GLFW_PRESS) {
        gameObject->m_position -= gameObject->Right() * velocity;
    }

    if (D == GLFW_PRESS) {
        gameObject->m_position += gameObject->Right() * velocity;
    }

    double currentX, currentY;
    glfwGetCursorPos(Application::GetCurrentWindow(), &currentX, &currentY);

    float xoffset = currentX - lastX;
    float yoffset = lastY - currentY;

    lastX = currentX;
    lastY = currentY;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    gameObject->m_eulerAngles.y += xoffset;
    gameObject->m_eulerAngles.x += yoffset;

    gameObject->m_eulerAngles.x = glm::clamp(gameObject->m_eulerAngles.x, -89.0f, 89.0f);
}
