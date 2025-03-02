#include "Transform.h"

Transform::Transform() {
}

Transform::~Transform() {
}


glm::vec3 Transform::Right() const {
    return normalize(cross(Forward(), glm::vec3(0, 1, 0)));
}

glm::vec3 Transform::Up() const {
    return normalize(cross(Right(), Forward()));
}

glm::vec3 Transform::Forward() const {
    glm::vec3 front;

    float Yaw = m_eulerAngles.y;
    float Pitch = m_eulerAngles.x;

    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    return normalize(front);
}
