#pragma once

#include <memory>
#include <string>
#include <vec3.hpp>
#include <vector>

#include "Texture/Texture.h"

struct Vertex;
class Material;
class IndexBuffer;
class VertexBuffer;
class VertexArray;

class Mesh {
public:
    Mesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<std::shared_ptr<Texture> > textures);

    std::vector<std::shared_ptr<Mesh> > children;
    Mesh *parent;

    std::vector<Vertex> m_vertices;
    glm::vec3 m_origin;
    std::vector<unsigned int> m_indices;
    std::vector<std::shared_ptr<Texture> > m_textures;
    std::vector<std::shared_ptr<Material> > m_materials;
    std::string m_name;

    void AddChild(std::shared_ptr<Mesh> child);;
};
