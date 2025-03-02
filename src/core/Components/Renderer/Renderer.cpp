#include "Renderer.h"

#include <iostream>
#include <GL/glew.h>

#include "core/Material.h"
#include "core/Mesh/Mesh.h"
#include "core/Model/Model.h"
#include "IndexBuffer/IndexBuffer.h"
#include "Shader/Shader.h"
#include "VertexArray/VertexArray.h"
#include "VertexBuffer/VertexBuffer.h"
#include "VertexBufferLayout/VertexBufferLayout.h"

Renderer::Renderer(const std::vector<float> &vertexDataVector,
                   const std::vector<unsigned int> &indexBufferIndicesVector,
                   const Material &material) {
    //VERTEX BUFFER
    m_VBO = std::make_unique<VertexBuffer>(vertexDataVector.data(), vertexDataVector.size());

    //LAYOUT
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<float>(3); //vertex pos
    vertex_buffer_layout.Push<float>(3); //vertex normals
    vertex_buffer_layout.Push<float>(2); //uvCoord

    //VERTEX ARRAY
    m_VAO = std::make_unique<VertexArray>();
    m_VAO->AddBuffer(*m_VBO, vertex_buffer_layout);

    //ELEMENT BUFFER
    m_IBO = std::make_unique<IndexBuffer>(indexBufferIndicesVector.data(), indexBufferIndicesVector.size());

    m_materials.push_back(material);
}

Renderer::Renderer(std::shared_ptr<Mesh> &mesh) {
    std::vector<float> verticesTest;

    for (auto m_vertex: mesh->m_vertices) {
        verticesTest.push_back(m_vertex.Position.x);
        verticesTest.push_back(m_vertex.Position.y);
        verticesTest.push_back(m_vertex.Position.z);

        verticesTest.push_back(m_vertex.Normal.x);
        verticesTest.push_back(m_vertex.Normal.y);
        verticesTest.push_back(m_vertex.Normal.z);

        verticesTest.push_back(m_vertex.TexCoords.x);
        verticesTest.push_back(m_vertex.TexCoords.y);
    }

    //VERTEX BUFFER
    m_VBO = std::make_shared<VertexBuffer>(verticesTest.data(), verticesTest.size());

    //LAYOUT
    VertexBufferLayout vertex_buffer_layout;
    vertex_buffer_layout.Push<float>(3); //vertex pos
    vertex_buffer_layout.Push<float>(3); //vertex normals
    vertex_buffer_layout.Push<float>(2); //uvCoord

    //VERTEX ARRAY
    m_VAO = std::make_shared<VertexArray>();
    m_VAO->AddBuffer(*m_VBO, vertex_buffer_layout);

    std::vector<unsigned int> indicesTest;

    for (unsigned m_index: mesh->m_indices) {
        indicesTest.push_back(m_index);
    }
    //ELEMENT BUFFER
    m_IBO = std::make_unique<IndexBuffer>(indicesTest.data(), indicesTest.size());
}

Renderer::~Renderer() {
}

void Renderer::start() {
}

void Renderer::update() {
    m_VAO->Bind();
    m_IBO->Bind();

    for (const auto &m_material: m_materials) {
        m_material.GetShader()->Bind();

        m_material.setModelTransform(gameObject->GetGlobalPosition(), gameObject->GetGlobalEulerAngles(),
                                     gameObject->GetGlobalScale());
        m_material.updateMaterialProperties();
        m_material.updateViewMatrix();
        m_material.updateLights();
    }

    glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::AddMaterial(const Material &material) {
    m_materials.push_back(material);
}

