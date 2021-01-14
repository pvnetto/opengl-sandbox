#include "02_VBO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void LOGL_02_VBO::OnAttach() {
    // 0) Declares VAO and VBOs + attributes
	float vertices[] = {
        // Triangle 1
        -0.8f, -0.2f, 0.0f,     // left
	    -0.4f, -0.2f, 0.0f,     // right
	    -0.6f, 0.2f, 0.0f,      // top

        // Triangle 2
        0.0f, -0.2f, 0.0f,     // left
	    0.4f, -0.2f, 0.0f,     // right
	    0.2f, 0.2f, 0.0f,      // top
	};

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// Binds VAO, so all subsequent buffers are bound to it
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

    // glVertexAttribPointer should tell the shader how to read buffer data
    // In this case we are declaring vec3 parameters as subsequent float values in an array,
    // so size = 3, type = float and stride = 3 * sizeof(float)
	// Also, Attributes are declared on slot 0, so the shader should use layout = 0 to find them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// 1) Declares and compiles vertex shaders
	std::string vertexShaderSource = "";
	vertexShaderSource += "#version 330 core\n";
	vertexShaderSource += "layout (location = 0) in vec3 aPosition;\n";
	vertexShaderSource += "void main() {\n";
	vertexShaderSource += "gl_Position = vec4(aPosition, 1.0f);\n";
	vertexShaderSource += "}\0";

	auto vertexShaderSourceC = vertexShaderSource.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
	glCompileShader(vertexShader);

	// (OPTIONAL) Checks shader for errors
	int vertSuccess;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertSuccess);
	if (!vertSuccess) {
		char info[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, info);
		std::cout << "Failed to compile vertex shader: " << info << "\n";
		return;
	}

	// 2) Declares and compiles fragment shaders
	std::string fragmentShaderSrc = "";
	fragmentShaderSrc += "#version 330 core\n";
	fragmentShaderSrc += "out vec4 fragmentColor;\n";
	fragmentShaderSrc += "void main() {\n";
	fragmentShaderSrc += "fragmentColor = vec4(0.2f, 0.3f, 1.0f, 1.0f);\n";
	fragmentShaderSrc += "}\0";

	auto fragmentShaderSrcC = fragmentShaderSrc.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrcC, NULL);
	glCompileShader(fragmentShader);

	// (OPTIONAL) Checks shader for errors
	int fragSuccess;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragSuccess);

	if (!fragSuccess) {
		char info[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, info);
		std::cout << "Failed to compile fragment shader: " << info << "\n";
		return;
	}

	// 3) Declares shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);

	// (OPTIONAL) Check for program errors
	int programSuccess;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &programSuccess);

	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return;
	}

	// (OPTIONAL) Deletes all shaders included in shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);    
}

void LOGL_02_VBO::OnUpdate() {
    // 4) Use shader program, bind VAO and draw the triangle
    glUseProgram(m_shaderProgram);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);	// Vertices are drawn counter-clockwise when using glDrawArrays 
}
