#include "Model.h"

#include <imgui_impl_opengl3_loader.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "core/Components/Renderer/Renderer.h"
#include "core/Mesh/Mesh.h"
#include "core/Structs/Vertex.h"
#include "Texture/Texture.h"

Model::Model(const char *path) {
    loadModel(path);
}

std::shared_ptr<GameObject> Model::GenerateGameObject() {
    std::shared_ptr<GameObject> rootGameObj = std::make_shared<GameObject>(m_name);

    std::shared_ptr<Mesh> rootMesh = std::make_shared<Mesh>();
    rootMesh->children = m_meshes;

    GenerateGameObjectRecursive(rootMesh, rootGameObj);

    return rootGameObj;
}

void Model::GenerateGameObjectRecursive(std::shared_ptr<Mesh> &mesh, std::shared_ptr<GameObject> &parent) {
    for (std::shared_ptr<Mesh> &currentMesh: mesh->children) {
        std::shared_ptr<GameObject> meshGameObject = std::make_shared<GameObject>(currentMesh->m_name);
        // meshGameObject->m_position = mesh->m_origin;

        auto renderer = std::make_shared<Renderer>(currentMesh);

        for (const auto &m_texture: currentMesh->m_textures) {
            Material material = Material(nullptr);
            material.AddTexture(*m_texture);

            renderer->AddMaterial(material);
        }

        if (currentMesh->m_textures.empty()) {
            renderer->AddMaterial(Material(nullptr));
        }

        meshGameObject->AddComponent(renderer);

        parent->AddChild(meshGameObject);

        if (!currentMesh->children.empty()) {
            GenerateGameObjectRecursive(currentMesh, meshGameObject);
        }
    }
}


void Model::loadModel(std::string path) {
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ASSIMP ERROR -> " << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory = path.substr(0, path.find_last_of('/'));

    m_name = path.substr(path.find_last_of('/') + 1, path.find_last_of('.') - path.find_last_of('/') - 1);
    processNodeRecursive(scene->mRootNode, scene);
}

void Model::processNodeRecursive(aiNode *node, const aiScene *scene, std::shared_ptr<Mesh> parent) {
    // Calcule a transformação global do nó atual
    aiMatrix4x4 globalTransform = node->mTransformation;

    // Propague transformações dos pais, se existirem
    aiNode* current = node->mParent;
    while (current) {
        globalTransform = current->mTransformation * globalTransform;
        current = current->mParent;
    }

    if (node->mNumMeshes > 0) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

            auto currentMesh = std::make_shared<Mesh>(processMesh(mesh, scene));
            currentMesh->m_name = mesh->mName.C_Str();

            aiVector3D globalPosition = aiVector3D(globalTransform.a4, globalTransform.b4, globalTransform.c4);

            currentMesh->m_origin = glm::vec3(globalPosition.x, globalPosition.y, globalPosition.z);

            if (parent) {
                parent->AddChild(currentMesh);
            } else {
                m_meshes.push_back(currentMesh);
            }
            for (unsigned int j = 0; j < node->mNumChildren; j++) {
                processNodeRecursive(node->mChildren[j], scene, currentMesh);
            }
        }
    } else {
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNodeRecursive(node->mChildren[i], scene, parent);
        }
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture> > textures;

    //VERTEX DATA
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // positions
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        // // normals
        if (mesh->HasNormals()) {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    //INDEX BUFFER
    //fill the index buffer, since we are loading the model with the configuration aiProcess_Triangulate, all the faces
    //will be made of triangles, we can just read them in order and add to out index vector
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    //MATERIAL DATA
    // read material data
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<Texture> > diffuseMaps = loadTextures(
            material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return {vertices, indices, textures};
}

std::vector<std::shared_ptr<Texture> > Model::loadTextures(aiMaterial *mat, aiTextureType type,
                                                           std::string typeName) {
    std::vector<std::shared_ptr<Texture> > meshTextures;
    for (unsigned int textureSlot = 0; textureSlot < mat->GetTextureCount(type); textureSlot++) {
        aiString str;
        mat->GetTexture(type, textureSlot, &str);

        bool skipTexture = false;
        //check if should skip the load of the texture if we already loaded (comparing the path of both)
        for (unsigned int j = 0; j < m_textures.size(); j++) {
            //if strcmp returns 0 this means that the paths are equal, we don't need to load the texture
            //and we add the reference to the loaded texture to the return list
            if (std::strcmp(m_textures[j]->path.C_Str(), str.C_Str()) == 0) {
                meshTextures.push_back(m_textures[j]);
                skipTexture = true;
                break;
            }
        }

        if (!skipTexture) {
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(str.C_Str(), m_directory);
            texture->type = typeName;
            texture->path = str;
            texture->textureSlot = GL_TEXTURE0 + textureSlot;

            m_textures.push_back(texture);
            meshTextures.push_back(texture);
        }
    }
    return meshTextures;
}
