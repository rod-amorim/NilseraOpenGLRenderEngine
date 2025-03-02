#include "GameObject.h"

#include "Components/IComponent.h"
#include "VertexBufferLayout/VertexBufferLayout.h"


GameObject::GameObject(const std::string &name, GameObject *parent) : name(name), parent(parent) {
}

GameObject::~GameObject() {
}


void GameObject::AddChild(const std::shared_ptr<GameObject> child) {
    child->parent = this;
    children.push_back(child);
}

void GameObject::removeChildByName(const std::string &childName) {
    std::erase_if(children, [&childName](const std::shared_ptr<GameObject> &child) {
        return child->name == childName;
    });
}

void GameObject::AddComponent(std::shared_ptr<IComponent> component) {
    component->gameObject = this;
    components.push_back(component);
}

glm::vec3 GameObject::GetGlobalPosition() const {
    if (parent) {
        return parent->GetGlobalPosition() + m_position;
    }
    return m_position;
}

glm::vec3 GameObject::GetGlobalEulerAngles() const {
    if (parent) {
        // get parent rot
        glm::vec3 parentRotation = parent->GetGlobalEulerAngles();

        // sums the rotation with the parent
        return parentRotation + m_eulerAngles;
    }
    return m_eulerAngles;
}

glm::vec3 GameObject::GetGlobalScale() const {
    if (parent) {
        return parent->GetGlobalScale() * m_scale;
    }
    return m_scale;
}
