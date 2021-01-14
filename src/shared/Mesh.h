#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Shader.h"
#include "Texture2D.h"

// Defining vertices as structs is useful to take advantage of the 'offsetof' macro
struct Vertex {
    glm::vec3 Position;
    glm::vec2 UV;
    glm::vec3 Normal;

    Vertex(float p1, float p2, float p3, float uv1, float uv2, float n1, float n2, float n3) {
        Position = glm::vec3(p1, p2, p3);
        UV = glm::vec2(uv1, uv2);
        Normal = glm::vec3(n1, n2, n3);
    }
};

class Mesh {
public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;

    Mesh() = default;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures );

    void Draw(Shader& shader);

private:
    unsigned int m_vao, m_vbo, m_ebo;
    void initializeMesh();
};