#include "LightManager.h"
#include <algorithm>

void LightManager::AddDirectionalLight(DirectionalLight *light) {
    loaded_directional_lights.push_back(light);
}

void LightManager::AddPointLight(PointLight *light) {
    loaded_point_lights.push_back(light);
}

void LightManager::AddSpotLightLight(SpotLight *light) {
    loaded_spot_lights.push_back(light);
}

void LightManager::RemoveDirectionalLight(DirectionalLight *light) {
    const auto it = std::ranges::remove_if(loaded_directional_lights,
                                           [light](DirectionalLight *l) { return l == light; }).begin();

    if (it != loaded_directional_lights.end()) {
        loaded_directional_lights.erase(it, loaded_directional_lights.end());
    }
}

void LightManager::RemovePointLight(PointLight *light) {
    const auto it = std::ranges::remove_if(loaded_point_lights,
                                           [&light](PointLight *l) { return l == light; }).begin();

    if (it != loaded_point_lights.end()) {
        loaded_point_lights.erase(it, loaded_point_lights.end());
    }
}

void LightManager::RemoveSpotLightLight(SpotLight *light) {
    const auto it = std::ranges::remove_if(loaded_spot_lights,
                                           [&light](SpotLight *l) { return l == light; }).begin();

    if (it != loaded_spot_lights.end()) {
        loaded_spot_lights.erase(it, loaded_spot_lights.end());
    }
}
