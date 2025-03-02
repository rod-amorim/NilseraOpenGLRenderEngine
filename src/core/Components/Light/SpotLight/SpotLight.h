#pragma once
#include <memory>

#include "core/Components/Light/Light.h"

class SpotLight : public Light, std::enable_shared_from_this<SpotLight> {
public:
    SpotLight();

    SpotLight(float intensity);

    ~SpotLight();

    void start() override;

    void update() override;

    [[nodiscard]] std::string getName() const override {
        return "SpotLight";
    }

    float m_range = 10.0f;
    float m_innerCutOff = 10;
    float m_outerCutOff = 25;
};
