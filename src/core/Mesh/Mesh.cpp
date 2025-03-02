#include "Mesh.h"

#include "VertexBufferLayout/VertexBufferLayout.h"

Mesh::Mesh() {
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures) {
    this->m_vertices = vertices;
    this->m_indices = indices;
    this->m_textures = textures;
}

void Mesh::AddChild(std::shared_ptr<Mesh> child) {
    child->parent = this;
    children.push_back(child);
}
