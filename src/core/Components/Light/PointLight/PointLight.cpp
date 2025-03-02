#include "PointLight.h"

#include "core/LightManager/LightManager.h"

PointLight::PointLight() {
    LightManager::instance().AddPointLight(this);

}

PointLight::PointLight(float intensity) {
    m_intensity = intensity;
    LightManager::instance().AddPointLight(this);
}

PointLight::~PointLight() {
    LightManager::instance().RemovePointLight(this);
}

void PointLight::start() {
    Light::start();
}

void PointLight::update() {
    Light::update();
}
