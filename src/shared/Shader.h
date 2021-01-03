#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);

    void Use();

    void SetBool(const std::string& uniformName, bool val) const;
    void SetInt(const std::string& uniformName, int val) const;
    void SetFloat(const std::string& uniformName, float val) const;
    void SetMatrix(const std::string& uniformName, glm::mat4 mat);
    void SetVector3(const std::string& uniformName, glm::vec3 vec);

private:
	unsigned int m_id;

    std::string ReadShader(const char* path);
    const unsigned int CompileShader(const char* code, GLenum type);

};