#pragma once
#include <memory>
#include <vec3.hpp>

#include "GameObject.h"

class GameObject;

class Scene {
private:
    std::shared_ptr<GameObject> rootGameObject;

    void UpdateRecursive(std::shared_ptr<GameObject> &node);

    void StartRecursive(std::shared_ptr<GameObject> &node);

public:
    explicit Scene(const std::string &name = "Scene");

    glm::vec3 ambientLight = glm::vec3(1.0f, 1.0f, 1.0f);

    void AddGameObject(const std::shared_ptr<GameObject> &game_object) const;

    void UpdateGameObjects();

    void StartGameObjects();

    std::shared_ptr<GameObject> GetRootGameObject() {
        return rootGameObject;
    }

    virtual void Setup() = 0;
};
