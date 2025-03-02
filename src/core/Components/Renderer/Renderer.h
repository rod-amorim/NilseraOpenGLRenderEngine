#pragma once
#include <vector>

#include "core/Material.h"
#include "core/Components/IComponent.h"
class Mesh;
class Model;
class VertexBuffer;
class GameObject;
class Shader;
class VertexArray;
class IndexBuffer;

class Renderer : public IComponent {
public:
    Renderer(const std::vector<float> &vector, const std::vector<unsigned int> &indices, const Material &material);
    Renderer(std::shared_ptr<Mesh> &model);

    ~Renderer();

    void start() override;

    void update() override;
    [[nodiscard]] std::string getName() const override {
        return "Renderer";
    }

    void AddMaterial(const Material & material);

private:
    std::shared_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::shared_ptr<IndexBuffer> m_IBO;
    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<Texture> m_Texture;

    std::vector<Material> m_materials;
};
