#pragma once
#include <memory>

#include "core/Components/Light/Light.h"

class PointLight : public Light, std::enable_shared_from_this<PointLight> {
public:
    PointLight();

    explicit PointLight(float intensity);

    ~PointLight();

    void start() override;

    void update() override;

    [[nodiscard]] std::string getName() const override {
        return "PointLight";
    }

    float m_range = 10.0f;
};
