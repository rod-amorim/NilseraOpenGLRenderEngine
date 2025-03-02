#pragma once
#include <memory>

class Input;
class Renderer;
struct GLFWwindow;

class Application {
public:
    Application();

    ~Application();

    void Run();
    inline static void SetCurrentWindow(GLFWwindow* window) { m_Window = window; }
    inline static GLFWwindow* GetCurrentWindow() { return m_Window; }

private:
    static GLFWwindow* m_Window;

    std::unique_ptr<Input> input;

    float lastFrameTime = 0.f;

    void Start();

    void Update();

    void RenderImGui();
};
