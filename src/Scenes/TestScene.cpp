#include "TestScene.h"

#include <iostream>

#include "core/Material.h"
#include "core/SceneManager.h"
#include "core/Components/Behaviour/FirstPersonCharacterController.h"
#include "core/Components/Camera/Camera.h"
#include "core/Components/Light/DirectionalLight/DirectionalLight.h"
#include "core/Components/Light/PointLight/PointLight.h"
#include "core/Components/Light/SpotLight/SpotLight.h"
#include "core/Components/Renderer/Renderer.h"
#include "core/LightManager/LightManager.h"
#include "core/Mesh/Mesh.h"
#include "core/Structs/Vertex.h"
#include "core/Model/Model.h"
#include "core/TMap/TMap.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"

// std::vector<float> verticesTest = {
//     1, 1, -1, 0, 1, 0, 0.625f, 0.5f, -1, 1, -1, 0, 1, 0, 0.875f, 0.5f, -1, 1, 1, 0, 1, 0, 0.875f, 0.25f, 1, 1, 1, 0, 1,
//     0, 0.625f, 0.25f, 1, -1, 1, 0, 0, 1, 0.375f, 0.25f, 1, 1, 1, 0, 0, 1, 0.625f, 0.25f, -1, 1, 1, 0, 0, 1, 0.625f, 0,
//     -1, -1, 1, 0, 0, 1, 0.375f, 0, -1, -1, 1, -1, 0, 0, 0.375f, 1, -1, 1,
//     1, -1, 0, 0, 0.625f, 1, -1, 1, -1, -1, 0, 0, 0.625f, 0.75f, -1, -1, -1, -1, 0, 0, 0.375f, 0.75f, -1, -
//     1,
//     -1, 0, -1, 0, 0.125f, 0.5f, 1, -1, -1, 0, -1, 0, 0.375f, 0.5f, 1, -1, 1, 0, -1, 0, 0.375f, 0.25f, -1, -1, 1, 0, -1,
//     0, 0.125f, 0.25f, 1, -1, -1, 1, 0, 0, 0.375f, 0.5f, 1, 1, -1, 1, 0, 0, 0.625f, 0.5f, 1, 1, 1, 1, 0, 0, 0.625f,
//     0.25f, 1, -1, 1, 1, 0, 0, 0.375f, 0.25f, -1, -1, -1, 0, 0, -1, 0.375f, 0.75f,
//     -1, 1, -1, 0, 0, -1, 0.625f, 0.75f, 1, 1, -1, 0, 0, -1, 0.625f, 0.5f, 1, -1, -1, 0, 0, -1, 0.375f, 0.5f
// };
std::vector<float> vertices = {
    // Frente (normais apontando para +Z)
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Inferior esquerdo
    0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Inferior direito
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Superior direito
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Superior esquerdo

    // Trás (normais apontando para -Z)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Inferior esquerdo
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Inferior direito
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // Superior direito
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // Superior esquerdo

    // Esquerda (normais apontando para -X)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Inferior esquerdo
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Inferior direito
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Superior direito
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // Superior esquerdo

    // Direita (normais apontando para +X)
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Inferior esquerdo
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Inferior direito
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Superior direito
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Superior esquerdo

    // Superior (normais apontando para +Y)
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Inferior esquerdo
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Inferior direito
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Superior direito
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Superior esquerdo

    // Inferior (normais apontando para -Y)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Inferior esquerdo
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // Inferior direito
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // Superior direito
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // Superior esquerdo
};


std::vector<float> triangle = {
    // Posições (x, y, z)       // Cores (r, g, b)
    0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Vértice superior
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // Vértice inferior esquerdo
    0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

std::vector<unsigned int> indicesTriangle = {
    0, 1, 2
};
//
// std::vector<unsigned int> indicesTest = {
//     0, 1, 2, 0, 2, 3, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22,
//     20, 22, 23
// };

std::vector<unsigned int> indices = {
    // Frente
    0, 1, 2, 2, 3, 0,
    // Trás
    4, 5, 6, 6, 7, 4,
    // Esquerda
    8, 9, 10, 10, 11, 8,
    // Direita
    12, 13, 14, 14, 15, 12,
    // Superior
    16, 17, 18, 18, 19, 16,
    // Inferior
    20, 21, 22, 22, 23, 20,
};

void TestScene::Setup() {
    ambientLight = glm::vec3(.1);
    SceneManager::getInstance().current = shared_from_this();

    //Camera
    std::shared_ptr<GameObject> cameraObject = std::make_shared<GameObject>("Camera");
    cameraObject->AddComponent(std::make_shared<Camera>());
    cameraObject->AddComponent(std::make_shared<FirstPersonCharacterController>());
    cameraObject->AddComponent(std::make_shared<SpotLight>(1));
    cameraObject->m_position = glm::vec3(0, 1, 0);
    this->AddGameObject(cameraObject);

    Model model = Model("res/Models/Shotgun.fbx");
    std::shared_ptr<GameObject> modelGameObject = model.GenerateGameObject();
    modelGameObject->m_position = glm::vec3(2, 2, 0);
    this->AddGameObject(modelGameObject);

    Model model2 = Model("res/Models/Primitives/Sphere.fbx");
    std::shared_ptr<GameObject> newModeGo = model2.GenerateGameObject();
    this->AddGameObject(newModeGo);
    //
    // Model ground = Model("res/Models/Primitives/Quad.fbx");
    // std::shared_ptr<GameObject> groundGameObject = ground.GenerateGameObject();
    // groundGameObject->m_position = glm::vec3(0, 0, 0);
    // groundGameObject->m_eulerAngles = glm::vec3(-90, 0, 0);
    // groundGameObject->m_scale = glm::vec3(100, 100, 100);
    // this->AddGameObject(groundGameObject);
}

TestScene::~TestScene() {
    std::cout << "Destroying TestScene" << std::endl;
}
