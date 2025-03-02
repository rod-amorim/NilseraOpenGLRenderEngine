#pragma once
#include <gtc/matrix_transform.hpp>
#include <memory>
#include <vec3.hpp>

#include "Scene.h"

class Texture;
class Shader;
class Material;

class Material {
private:
    void UpdateLightsRecursive();
public:
    explicit Material(std::shared_ptr<Shader> shader,
                      glm::vec3 tint = glm::vec3(1.0f, 1.0f, 1.0f),
                      float shininess = 32,
                      int diffuse_map = 0,
                      int specular_map = 1,
                      int emission_map = 2);

    void updateMaterialProperties() const;

    void updateViewMatrix() const;

    void updateLights() const;


    void setModelTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) const;

    void SetUniform(const std::string &name, float value) const;

    void SetUniform(const std::string &name, const glm::vec3 &value) const;

    void SetUniform(const std::string &name, const glm::mat4 &value) const;

    void SetUniform(const std::string &name, int value) const;

    void AddTexture(Texture &texture);

    std::shared_ptr<Shader> GetShader() const;

    std::string m_name;
    glm::vec3 m_tint;
    int m_diffuseMap;
    int m_specularMap;
    int m_emissionMap;
    float m_shininess;

private:
    std::shared_ptr<Shader> m_Shader;

    int m_textureCount = 0;

    void updateDirectionalLight(const std::shared_ptr<Scene> &scene) const;

    void updatePointLights(const std::shared_ptr<Scene> &scene) const;

    void updateSpotLights(const std::shared_ptr<Scene> &scene) const;

    glm::mat4 m_Proj{};
};
