#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    : m_vertices(vertices),
      m_indices(indices),
      m_textures(textures) {
	initializeMesh();
}

void Mesh::initializeMesh() {
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);

    // binds vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    // binds ebo
    if(m_indices.size() > 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    }

    // initializes vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)offsetof(Vertex, UV));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);

    // unbinds vao and vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader) {
    shader.Use();
    // binds textures to uniformes
    // assumes all textures are declared on material struct, with naming convention textureN
    for(int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        shader.SetInt("material.texture" + std::to_string(i), i);

        glBindTexture(GL_TEXTURE_2D, m_textures[i].ID);
    }

    // resets active texture unit to 0 (default)
    glActiveTexture(GL_TEXTURE0);

    // draws object
    glBindVertexArray(m_vao);
    if(m_indices.size() == 0)
        glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    else
        glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}