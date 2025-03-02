#include "Input.h"

#include <GLFW/glfw3.h>

#include "Application.h"

Input::Input() : quit(false) {}

Input::~Input() {}

void Input::ProcessInput() {
    if (glfwGetKey(Application::GetCurrentWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        quit = true;
    }
}

bool Input::ShouldQuit() const {
    return quit;
}

bool Input::IsKeyPressed(const int key) {
    return glfwGetKey(Application::GetCurrentWindow(), key) == GLFW_PRESS;
}