#include <memory>
#include <vector>

class SpotLight;
class DirectionalLight;
class PointLight;
class Light;

class LightManager {
private:
    LightManager() = default;

    std::vector<DirectionalLight *> loaded_directional_lights;
    std::vector<PointLight *> loaded_point_lights;
    std::vector<SpotLight *> loaded_spot_lights;

    LightManager(const LightManager &) = delete;

    LightManager &operator=(const LightManager &) = delete;

public:
    static LightManager &instance() {
        static LightManager instance;
        return instance;
    }

    void AddDirectionalLight(DirectionalLight *light);

    void AddPointLight(PointLight *light);

    void AddSpotLightLight(SpotLight *light);

    void RemoveDirectionalLight(DirectionalLight *light);

    void RemovePointLight(PointLight *light);

    void RemoveSpotLightLight(SpotLight *light);

    std::vector<DirectionalLight *> getDirectionalLights() {
        return loaded_directional_lights;
    }

    std::vector<PointLight *> getPointLights() {
        return loaded_point_lights;
    }

    std::vector<SpotLight *> getSpotLights() {
        return loaded_spot_lights;
    }
};
