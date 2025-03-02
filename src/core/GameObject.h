#include <memory>
#include <vector>

#include "Transform.h"

class Mesh;
class IComponent;
class Material;
class Material;
class Shader;
class IndexBuffer;
class VertexBuffer;
class VertexArray;
class Transform;

class GameObject : public Transform {
public:
    GameObject(const std::string &name, GameObject *parent = nullptr);

    ~GameObject();

    std::string name;
    std::vector<std::shared_ptr<GameObject> > children;
    GameObject *parent;
    std::vector<std::shared_ptr<IComponent> > components;

    void AddChild(std::shared_ptr<GameObject> game_object);

    void removeChildByName(const std::string &childName);

    void AddComponent(std::shared_ptr<IComponent> component);


    template<typename T>
    T *GetComponent() {
        for (auto &component: components) {
            if (T *specificComponent = dynamic_cast<T *>(component.get())) {
                return specificComponent;
            }
        }
        return nullptr;
    }

    glm::vec3 GetGlobalPosition() const;

    glm::vec3 GetGlobalEulerAngles() const;

    glm::vec3 GetGlobalScale() const;

private:
};
