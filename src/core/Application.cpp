#include "Application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <GL/glew.h>

#include "GameTime.h"

#include <GLFW/glfw3.h>

#include "Input.h"
#include "Material.h"
#include "SceneManager.h"
#include "Screen.h"
#include "Transform.h"
#include "Components/Camera/Camera.h"
#include "Components/Renderer/Renderer.h"
#include "Scenes/TestScene.h"
#include "Texture/Texture.h"

GLFWwindow *Application::m_Window = nullptr;

std::shared_ptr<GameObject> selectedGameObject = nullptr;

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam) {
    std::cerr << "OpenGL Debug Message:\n";
    std::cerr << "Message: " << message << "\n";

    std::cerr << "Source: ";
    switch (source) {
        case GL_DEBUG_SOURCE_API: std::cerr << "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cerr << "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cerr << "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: std::cerr << "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION: std::cerr << "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER: std::cerr << "Other";
            break;
    }
    std::cerr << "\nType: ";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR: std::cerr << "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cerr << "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cerr << "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY: std::cerr << "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE: std::cerr << "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER: std::cerr << "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP: std::cerr << "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP: std::cerr << "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER: std::cerr << "Other";
            break;
    }
    std::cerr << "\nSeverity: ";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH: std::cerr << "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM: std::cerr << "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW: std::cerr << "Low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cerr << "Notification";
            break;
    }
    std::cerr << "\nID: " << id << "\n" << std::endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Application::Application()
    : input(std::make_unique<Input>()) {
    // Initialize GLFW
    if (!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW3");

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *win = glfwCreateWindow(Screen::Width, Screen::Height, "Nilsera render engine", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        throw std::runtime_error("Failed to create window");
    }

    SetCurrentWindow(win);

    // Make the window's context current
    glfwMakeContextCurrent(GetCurrentWindow());
    //Enable callback on resize win
    glfwSetFramebufferSizeCallback(GetCurrentWindow(), framebuffer_size_callback);
    //Enable debug
    // glEnable(GL_DEBUG_OUTPUT);
    //Enable zBuffer
    glEnable(GL_DEPTH_TEST);

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    //hide cursor and lock in the middle of the screen
    glfwSetInputMode(GetCurrentWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Execute glew to get access to all the open gl functions from the graphic card */
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to create window");
    }

    // debug callback
    glDebugMessageCallback(glDebugOutput, nullptr);

    //IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

Application::~Application() {
    std::cout << "Destroying application" << std::endl;
}

void Application::Run() {
    Start();
    //Loop
    while (!glfwWindowShouldClose(GetCurrentWindow())) {
        GameTime::Time = static_cast<float>(glfwGetTime());

        GameTime::DeltaTime = GameTime::Time - lastFrameTime;
        lastFrameTime = GameTime::Time;

        input->ProcessInput();

        Update();

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(GetCurrentWindow());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Application::Start() {
    std::shared_ptr<TestScene> testScene = std::make_shared<TestScene>();
    testScene->Setup();

    testScene->StartGameObjects();
}


void Application::Update() {
    // glClearColor(.2f, .4f, .3f, 1.0f);
    glClearColor(.0f, .0f, .0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    SceneManager::getInstance().current->UpdateGameObjects();

    RenderImGui();
}

void RenderGameObjects(std::shared_ptr<GameObject> node, bool isRoot = false) {
    // Flags para o nó da árvore
    ImGuiTreeNodeFlags nodeFlags = 0;
    if (selectedGameObject == node) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    // Cria o nó da árvore com o nome do GameObject
    bool nodeClicked = ImGui::TreeNodeEx(node->name.c_str(), nodeFlags);

    // Quando o nó é clicado, seleciona o GameObject
    if (ImGui::IsItemClicked()) {
        selectedGameObject = node;
    }

    // Se o nó for expandido, renderiza os filhos (se houver)
    if (nodeClicked) {
        for (auto &child: node->children) {
            RenderGameObjects(child); // Chama recursivamente para os filhos
        }
        ImGui::TreePop();
    }
}


void Application::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glm::vec3 ambient_light = SceneManager::getInstance().current->ambientLight;
    float color[]{ambient_light.x, ambient_light.y, ambient_light.z};

    ImGui::Begin("Properties");
    ImGui::ColorPicker3("Ambient color ", color);
    ImGui::End();
    SceneManager::getInstance().current->ambientLight = glm::vec3(color[0], color[1], color[2]);

    ImVec2 hierarchySize(250, ImGui::GetIO().DisplaySize.y / 1.5f);
    ImVec2 hierarchyPos(ImGui::GetIO().DisplaySize.x - hierarchySize.x - 260, 10);
    ImGui::SetNextWindowPos(hierarchyPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(hierarchySize, ImGuiCond_Always);

    ImGui::Begin("Hierarchy");

    if (SceneManager::getInstance().current->GetRootGameObject()) {
        RenderGameObjects(SceneManager::getInstance().current->GetRootGameObject(), true);
    }

    ImGui::End();


    ImVec2 inspectorSize(250, ImGui::GetIO().DisplaySize.y / 1.5f);
    ImVec2 inspectorPos(ImGui::GetIO().DisplaySize.x - inspectorSize.x - 10, 10);
    ImGui::SetNextWindowPos(inspectorPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(inspectorSize, ImGuiCond_Always);
    if (ImGui::Begin("Inspector", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
        if (selectedGameObject) {
            ImGui::Text(selectedGameObject->name.c_str());
            ImGui::Text("----------------------------");

            // Editable Position
            ImGui::Text("Position");
            ImGui::DragFloat3("##Position", &selectedGameObject->m_position.x, 0.1f);

            // Editable Rotation
            ImGui::Text("Rotation");
            ImGui::DragFloat3("##Rotation", &selectedGameObject->m_eulerAngles.x, 0.1f);

            // Editable Scale
            ImGui::Text("Scale");
            ImGui::DragFloat3("##Scale", &selectedGameObject->m_scale.x, 0.1f);
        }
    }
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
