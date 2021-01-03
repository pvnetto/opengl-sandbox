#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
	Camera(glm::vec3 position, glm::vec3 rotation, glm::vec3 up);

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const { return m_projection; }

    glm::vec3 GetForward() const { return m_forward; }
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    glm::vec3 GetPosition() const { return m_position; }

    void SetPosition(glm::vec3 position) { m_position = position; }
    void SetRotation(glm::vec3 newRotation);

    void AddRotation(glm::vec3 rotationDelta);
    void AddPosition(glm::vec3 positionDelta) { m_position += positionDelta; }

    void SetOrthographic(float width, float height, float near, float far);
    void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
    
private:
    glm::vec3 m_position;
    glm::vec3 m_forward;
    glm::vec3 m_rotationEuler;

    glm::vec3 m_worldUp;

    glm::mat4 m_projection;

    void UpdateDirection();
};