#include "Light.h"

#include "core/LightManager/LightManager.h"

Light::Light() {
    this->m_intensity = 1;
    this->m_color = glm::vec3(1.0f, 1.0f, 1.0f);
}

Light::Light(float intensity, glm::vec3 color) {
    this->m_intensity = intensity;
    this->m_color = color;
}

void Light::start() {
}

void Light::update() {
}
