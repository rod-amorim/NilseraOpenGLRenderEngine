#include "Camera.h"
#include "core/SceneManager.h"

class Scene;

Camera *Camera::camera = nullptr;

Camera::Camera(): Front(WORLD_FRONT) {
    WorldUp = WORLD_UP;
    Yaw = YAW;
    Pitch = PITCH;
    camera = this;
}

void Camera::start() {
}

void Camera::update() {
}

glm::mat4 Camera::GetViewMatrix() const {
    // Vetores direcionais já calculados
    glm::vec3 forward = gameObject->Forward();  // Câmera olhando para a frente
    glm::vec3 right = gameObject->Right();      // Vetor direito (para a direita)
    glm::vec3 up = gameObject->Up();            // Vetor para cima

    // Posição da câmera
    glm::vec3 cameraPos = gameObject->m_position;

    // A view matrix pode ser calculada diretamente como:
    glm::mat4 view = glm::mat4(1.0f);

    // Preenche as colunas da view matrix com os vetores de direção
    view[0] = glm::vec4(right, 0.0f); // Coluna 0: Right
    view[1] = glm::vec4(up, 0.0f);    // Coluna 1: Up
    view[2] = glm::vec4(-forward, 0.0f); // Coluna 2: -Forward (direção da câmera invertida)
    view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); // Coluna 3: Translação (não aplicamos aqui)

    // Agora, aplicamos a translação com a posição da câmera
    view = glm::translate(view, -cameraPos);

    return view;
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = normalize(front);
    // also re-calculate the Right and Up vector
    Right = normalize(cross(Front, WorldUp));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = normalize(cross(Right, Front));
}
