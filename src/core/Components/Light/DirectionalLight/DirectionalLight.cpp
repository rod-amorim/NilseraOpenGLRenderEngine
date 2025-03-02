#include "DirectionalLight.h"

#include "core/LightManager/LightManager.h"

DirectionalLight::DirectionalLight() {
    LightManager::instance().AddDirectionalLight(this);
}

DirectionalLight::DirectionalLight(float intensity) {
    m_intensity = intensity;
    LightManager::instance().AddDirectionalLight(this);
}

DirectionalLight::~DirectionalLight() {
    LightManager::instance().RemoveDirectionalLight(this);
}

void DirectionalLight::start() {
    Light::start();
}

void DirectionalLight::update() {
    Light::update();
}
