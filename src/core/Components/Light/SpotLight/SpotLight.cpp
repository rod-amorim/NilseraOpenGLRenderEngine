#include "SpotLight.h"

#include "core/LightManager/LightManager.h"

SpotLight::SpotLight() {
    LightManager::instance().AddSpotLightLight(this);
}

SpotLight::SpotLight(float intensity) {
    m_intensity = intensity;
    LightManager::instance().AddSpotLightLight(this);
}

SpotLight::~SpotLight() {
    LightManager::instance().RemoveSpotLightLight(this);
}

void SpotLight::start() {
    Light::start();
}

void SpotLight::update() {
    Light::update();
}
