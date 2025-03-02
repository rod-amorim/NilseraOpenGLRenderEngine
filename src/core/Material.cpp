#include "Material.h"

#include <iostream>


#include "Shader/Shader.h"
#include "Texture/Texture.h"


#include <utility>

#include "Application.h"
#include "SceneManager.h"
#include "Components/Camera/Camera.h"

#include "Components/Light/Light.h"
#include "Components/Light/DirectionalLight/DirectionalLight.h"
#include "Components/Light/PointLight/PointLight.h"
#include "Components/Light/SpotLight/SpotLight.h"
#include "LightManager/LightManager.h"

class DirectionalLight;

void Material::UpdateLightsRecursive() {
}

Material::Material(std::shared_ptr<Shader> shader,
                   glm::vec3 tint,
                   float shininess,
                   int diffuse_map,
                   int specular_map,
                   int emission_map): m_tint(tint),
                                      m_textureCount(0),
                                      m_shininess(shininess),
                                      m_diffuseMap(diffuse_map),
                                      m_specularMap(specular_map),
                                      m_emissionMap(emission_map) {
    if (!shader) {
        shader = std::make_shared<Shader>("res/Shaders/PsxStyle/Psx_Vert.glsl",
                                          "res/Shaders/PsxStyle/Psx_Frag.glsl");
    }

    m_Shader = std::move(shader);

    SetUniform("material.tint", m_tint);

    SetUniform("material.shininess", m_shininess);

    SetUniform("material.specularMap", m_specularMap);
    SetUniform("material.diffuseMap", m_diffuseMap);
    SetUniform("material.emissionMap", m_emissionMap);
    SetUniform("textureCount", m_textureCount);
}

void Material::updateMaterialProperties() const {
    SetUniform("material.tint", m_tint);

    SetUniform("material.shininess", m_shininess);

    SetUniform("material.specularMap", m_specularMap);
    SetUniform("material.diffuseMap", m_diffuseMap);
    SetUniform("material.emissionMap", m_emissionMap);
    SetUniform("textureCount", m_textureCount);
}

void Material::updateViewMatrix() const {
    glm::mat4 view = lookAt(Camera::camera->gameObject->m_position,
                            Camera::camera->gameObject->m_position + Camera::camera->gameObject->Forward(),
                            glm::vec3(0.0f, 1.0f, 0.0f));

    SetUniform("cameraPos", Camera::camera->gameObject->m_position);

    SetUniform(std::string("u_view"), view);
}


void Material::updateLights() const {
    std::shared_ptr<Scene> currentScene = SceneManager::getInstance().current;

    SetUniform(std::string("ambientLight"), currentScene->ambientLight);

    updateDirectionalLight(currentScene);

    updatePointLights(currentScene);

    updateSpotLights(currentScene);
}

void Material::updateDirectionalLight(const std::shared_ptr<Scene> &scene) const {
    for (const DirectionalLight *light: LightManager::instance().getDirectionalLights()) {
        SetUniform("directionalLight.intensity", light->m_intensity);

        SetUniform("directionalLight.direction", light->gameObject->Forward());

        SetUniform("directionalLight.diffuse", light->m_color);
        SetUniform("directionalLight.specular", light->m_color);
    }
}

void Material::updatePointLights(const std::shared_ptr<Scene> &scene) const {
    int pointLightIndex = 0;

    for (const PointLight *pointLight: LightManager::instance().getPointLights()) {
        auto currentIndexString = std::to_string(pointLightIndex);

        SetUniform("pointLights[" + currentIndexString + "].position", pointLight->gameObject->m_position);

        SetUniform("pointLights[" + currentIndexString + "].diffuse", pointLight->m_color);
        SetUniform("pointLights[" + currentIndexString + "].specular", pointLight->m_color);
        SetUniform("pointLights[" + currentIndexString + "].intensity", pointLight->m_intensity);
        SetUniform("pointLights[" + currentIndexString + "].range", pointLight->m_range);

        pointLightIndex++;

        SetUniform("numPointLights", pointLightIndex);
    }
}

void Material::updateSpotLights(const std::shared_ptr<Scene> &scene) const {
    int spotLightIndex = 0;
    for (const SpotLight *spotLight: LightManager::instance().getSpotLights()) {
        auto currentIndexString = std::to_string(spotLightIndex);

        SetUniform("spotlights[" + currentIndexString + "].position", spotLight->gameObject->m_position);
        SetUniform("spotlights[" + currentIndexString + "].direction", spotLight->gameObject->Forward());
        SetUniform("spotlights[" + currentIndexString + "].innerCutOff",
                   glm::cos(glm::radians(spotLight->m_innerCutOff)));
        SetUniform("spotlights[" + currentIndexString + "].outerCutOff",
                   glm::cos(glm::radians(spotLight->m_outerCutOff)));

        SetUniform("spotlights[" + currentIndexString + "].diffuse", spotLight->m_color);
        SetUniform("spotlights[" + currentIndexString + "].specular", spotLight->m_color);

        SetUniform("spotlights[" + currentIndexString + "].intensity", spotLight->m_intensity);
        SetUniform("spotlights[" + currentIndexString + "].range", spotLight->m_range);

        spotLightIndex++;

        SetUniform("numSpotLights", spotLightIndex);
    }
}

void Material::setModelTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) const {
    auto model_matrix = glm::mat4(1.0f);

    model_matrix = translate(model_matrix, pos);

    model_matrix = rotate(model_matrix, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model_matrix = rotate(model_matrix, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model_matrix = rotate(model_matrix, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model_matrix = glm::scale(model_matrix, scale);

    SetUniform("u_model", model_matrix);
}

void Material::SetUniform(const std::string &name, float value) const {
    m_Shader->setUniform1f(name, value);
}

void Material::SetUniform(const std::string &name, const glm::vec3 &value) const {
    m_Shader->setVec3Uniform(name, value);
}

void Material::SetUniform(const std::string &name, const glm::mat4 &value) const {
    m_Shader->setUniformMatrix4fv(name, &value);
}

void Material::SetUniform(const std::string &name, const int value) const {
    m_Shader->setUniform1i(name, value);
}

void Material::AddTexture(Texture &texture) {
    m_textureCount++;
    texture.Bind();

    m_Shader->setUniform1i("texture_" + texture.textureSlot, texture.textureSlot);
}

std::shared_ptr<Shader> Material::GetShader() const {
    return m_Shader;
}
