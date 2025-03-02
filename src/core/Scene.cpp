#include "Scene.h"

#include "Components/IComponent.h"


Scene::Scene(const std::string &name) {
    rootGameObject = std::make_shared<GameObject>(name);
}

void Scene::AddGameObject(const std::shared_ptr<GameObject> &game_object) const {
    rootGameObject->children.push_back(game_object);
}

void Scene::UpdateGameObjects() {
    UpdateRecursive(rootGameObject);
}

void Scene::StartGameObjects() {
    StartRecursive(rootGameObject);
}

void Scene::UpdateRecursive(std::shared_ptr<GameObject> &node) {
    // Porcess current node
    for (auto &&component: node->components) {
        component->update();
    }

    // Call children recursive
    for (std::shared_ptr<GameObject> &child: node->children) {
        UpdateRecursive(child);
    }
}

void Scene::StartRecursive(std::shared_ptr<GameObject> &node) {
    // Porcess current node
    for (auto &&component: node->components) {
        component->start();
    }

    // Call children recursive
    for (std::shared_ptr<GameObject> &child: node->children) {
        StartRecursive(child);
    }
}
