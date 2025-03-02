#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "core/Components/IComponent.h"
// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 VEC_ZERO = glm::vec3(0.0f, 0.0f, 0.0f);

class Camera : public IComponent {
public:
    explicit Camera();

    void start() override;

    void update() override;

    std::string getName() const override {
        return "Camera";
    }

    glm::mat4 GetViewMatrix() const;

    void updateCameraVectors();

    // euler Angles
    mutable float Yaw;
    mutable float Pitch;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    static Camera *camera;
};
