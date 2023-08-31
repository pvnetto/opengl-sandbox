#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct PerspectiveProps {
    float FOV = 90.f, Near = 0.1f, Far = 100.f;
};

struct OrthographicProps {
    float Near = 0.1f, Far = 100.f, Size = 4.f;
};

class Camera {
public:
    Camera() {}
	Camera(glm::vec3 position, glm::vec3 rotation, glm::vec3 up);

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const { return m_projection; }

    glm::vec3 GetForward() const { return m_Forward; }
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    glm::vec3& GetPosition() { return m_Position; }
    glm::vec3 GetRotationEuler() { return m_rotationEuler; }

    void SetPosition(glm::vec3 position) { m_Position = position; }
    void SetRotation(glm::vec3 newRotation);

    void AddRotation(glm::vec3 rotationDelta);
    void AddPosition(glm::vec3 positionDelta) { m_Position += positionDelta; }

    void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
    void SetOrthographic(float sizeX, float sizeY, float near, float far);
    
private:
    glm::vec3 m_Position;
    glm::vec3 m_Forward;
    glm::vec3 m_rotationEuler;

    glm::vec3 m_WorldUp;

    glm::mat4 m_projection;

    void UpdateDirection();
};