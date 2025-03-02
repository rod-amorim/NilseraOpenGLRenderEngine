#pragma once
#include "core/Components/IComponent.h"
#include "core/Components/Camera/Camera.h"

class FirstPersonCharacterController : public IComponent {
public:
    void start() override;

    void update() override;

    std::string getName() const override {
        return "FirstPersonCharacterController";
    }

private:
    const float speed = 1;
    const float sensitivity = 0.1f;

    double lastX, lastY;
};
