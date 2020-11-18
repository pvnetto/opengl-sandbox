#pragma once

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
	Shader(const char* vertexPath, const char* fragmentPath);

    void Use();

    void SetBool(const std::string& uniformName, bool val) const;
    void SetInt(const std::string& uniformName, int val) const;
    void SetFloat(const std::string& uniformName, float val) const;

private:
	unsigned int id;

    std::string ReadShader(const char* path);
    const unsigned int CompileShader(const char* code, GLenum type);

};