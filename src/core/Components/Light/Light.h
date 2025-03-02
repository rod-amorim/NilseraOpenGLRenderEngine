#pragma once
#include <vec3.hpp>

#include "core/Components/IComponent.h"

class Light : public IComponent {
public:
    Light();

    Light(float intensity, glm::vec3 color);

    void start() override;

    void update() override;

    [[nodiscard]] std::string getName() const override {
        return "Light";
    }

    float m_intensity = 1.0f;
    glm::vec3 m_color;
};
