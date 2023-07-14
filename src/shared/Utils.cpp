#include "Utils.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace utils {

    std::string readShaderFile(const char* path) {
        std::string shaderCode;
    
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            shaderFile.open(path);

            if(shaderFile.fail()) {
                std::cout << "::ERROR: File not found " << path << "\n";
            }

            std::stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();

            shaderFile.close();
            shaderCode = shaderStream.str();
        }
        catch(std::ifstream::failure e) {
            std::cout << "::ERROR: Failed to read shader from " << path << "\n";
        }

        return shaderCode;
    }

}