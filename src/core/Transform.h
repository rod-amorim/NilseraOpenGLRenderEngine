#pragma once
#include <fwd.hpp>
#include <detail/type_quat.hpp>
#include <gtc/matrix_transform.hpp>


class Transform {
public:
    Transform();

    ~Transform();

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_eulerAngles = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);

    [[nodiscard]] glm::quat EulerToQuaternion() const;

    [[nodiscard]] glm::vec3 Up() const;

    [[nodiscard]] glm::vec3 Right() const;

    [[nodiscard]] glm::vec3 Forward() const;

private:
};
