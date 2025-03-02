#pragma once
#include <memory>
#include <string>
#include <vector>
#include <assimp/material.h>

#include "core/Material.h"

class Texture;
struct aiMesh;
struct aiScene;
struct aiNode;
class Shader;
class Mesh;

class Model {
public:
    explicit Model(const char *path);

    std::vector<std::shared_ptr<Material> > GetMaterials() {
        return m_materials;
    }

    std::shared_ptr<GameObject> GenerateGameObject();
    std::string m_name;

private:
    std::vector<std::shared_ptr<Mesh> > m_meshes;
    std::string m_directory;
    std::vector<std::shared_ptr<Texture> > m_textures;
    std::vector<std::shared_ptr<Material> > m_materials;

    void loadModel(std::string path);

    void processNodeRecursive(aiNode *node, const aiScene *scene, std::shared_ptr<Mesh> parent = nullptr);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    void GenerateGameObjectRecursive(std::shared_ptr<Mesh> &mesh, std::shared_ptr<GameObject> &parent);

    std::vector<std::shared_ptr<Texture> > loadTextures(aiMaterial *mat, aiTextureType type,
                                                        std::string typeName);
};
