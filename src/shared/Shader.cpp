#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // Reads shader files 
    std::string vertexCode = ReadShader(vertexPath);
    std::string fragmentCode = ReadShader(fragmentPath);

    // Compiles both shaders
    unsigned int vertexShaderID = CompileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShaderID = CompileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

    // Creates program with both shaders
    id = glCreateProgram();
    glAttachShader(id, vertexShaderID);
    glAttachShader(id, fragmentShaderID);
    glLinkProgram(id);

    int linkSuccess;
    glGetProgramiv(id, GL_LINK_STATUS, &linkSuccess);

    if(!linkSuccess) {
        char linkInfo[512];
        glGetProgramInfoLog(id, 512, NULL, linkInfo);
        std::cout << "Failed to link shader " << linkInfo << "\n";
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::Use() {
    glUseProgram(id);
}

void Shader::SetBool(const std::string& uniformName, bool val) const {
    glUniform1i(glGetUniformLocation(id, uniformName.c_str()), (int) val);
}

void Shader::SetInt(const std::string& uniformName, int val) const {
    glUniform1i(glGetUniformLocation(id, uniformName.c_str()), val);
}

void Shader::SetFloat(const std::string& uniformName, float val) const {
    glUniform1f(glGetUniformLocation(id, uniformName.c_str()), val);
}

std::string Shader::ReadShader(const char* path) {
    std::string shaderCode;
    
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shaderFile.open(path);

        if(shaderFile.fail()) {
            std::cout << "File not found " << path << "\n";
        }

        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();

        shaderFile.close();
        shaderCode = shaderStream.str();
    }
    catch(std::ifstream::failure e) {
        std::cout << "Error: Failed to read shader from " << path << "\n";
    }

    return shaderCode;
}

const unsigned int Shader::CompileShader(const char* code, GLenum type) {
    unsigned int shaderID = glCreateShader(type);
    glShaderSource(shaderID, 1, &code, NULL);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    if(!success) {
        char info[512];
        glGetShaderInfoLog(shaderID, 512, NULL, info);
        std::cout << "Failed to compile shader: " << info << "\n";
    }

    return shaderID;
}