#pragma once
#include <memory>
#include <string>

#include "core/Components/Light/Light.h"

class DirectionalLight : public Light , std::enable_shared_from_this<DirectionalLight> {
public:
    DirectionalLight();
    DirectionalLight(float intensity);

    ~DirectionalLight();

    void start() override;

    void update() override;

    [[nodiscard]] std::string getName() const override {
        return "DirectionalLight";
    }
};
