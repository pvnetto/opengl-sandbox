#include "02_VAO.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void LOGL_02_VAO::OnAttach() {
	// 0) Declares VAO and VBOs + attributes
	float verticesA[] = {
	    // Triangle 1
	    -0.8f, -0.2f, 0.0f, // left
	    -0.4f, -0.2f, 0.0f, // right
	    -0.6f, 0.2f, 0.0f,  // top
	};

	float verticesB[] = {
	    // Triangle 2
	    0.0f, -0.2f, 0.0f, // left
	    0.4f, -0.2f, 0.0f, // right
	    0.2f, 0.2f, 0.0f,  // top
	};

    unsigned int vboA, vboB;
	glGenVertexArrays(1, &m_vaoA);
	glGenBuffers(1, &vboA);

	// Binds VAO, so all subsequent buffers are bound to it
	glBindVertexArray(m_vaoA);

	// Binds VBO after VAO
	glBindBuffer(GL_ARRAY_BUFFER, vboA);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesA), verticesA, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	// glVertexAttribPointer should tell the shader how to read buffer data
	// In this case we are declaring vec3 parameters as subsequent float values in an array,
	// so size = 3, type = float and stride = 3 * sizeof(float)
	// Also, Attributes are declared on slot 0, so the shader should use layout = 0 to find them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// Unbinds buffers so they're not accidentally used
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Sets second VAO
	glGenVertexArrays(1, &m_vaoB);
	glGenBuffers(1, &vboB);
	glBindVertexArray(m_vaoB);

	glBindBuffer(GL_ARRAY_BUFFER, vboB);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesB), verticesB, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

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

	// EXERCISE) Declares and compiles another fragment shader
	std::string yellowFragShaderSrc = "";
	yellowFragShaderSrc += "#version 330 core\n";
	yellowFragShaderSrc += "out vec4 fragmentColor;\n";
	yellowFragShaderSrc += "void main() {\n";
	yellowFragShaderSrc += "fragmentColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n";
	yellowFragShaderSrc += "}\0";

	auto yellowFragShaderSrcC = yellowFragShaderSrc.c_str();

	unsigned int yellowFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragShader, 1, &yellowFragShaderSrcC, NULL);
	glCompileShader(yellowFragShader);

	// (OPTIONAL) Checks shader for errors
	int yellowFragSuccess;
	glGetShaderiv(yellowFragShader, GL_COMPILE_STATUS, &yellowFragSuccess);

	if (!yellowFragSuccess) {
		char info[512];
		glGetShaderInfoLog(yellowFragShader, 512, NULL, info);
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

	// EXERCISE) Declares another shader program
	m_yellowShaderProgram = glCreateProgram();
	glAttachShader(m_yellowShaderProgram, vertexShader);
	glAttachShader(m_yellowShaderProgram, yellowFragShader);
	glLinkProgram(m_yellowShaderProgram);

	// (OPTIONAL) Check for program errors
	int yellowProgramSuccess;
	glGetProgramiv(m_yellowShaderProgram, GL_LINK_STATUS, &yellowProgramSuccess);

	if (!yellowProgramSuccess) {
		char info[512];
		glGetProgramInfoLog(m_yellowShaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return;
	}

	// (OPTIONAL) Deletes all shaders included in shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(yellowFragShader);
}

void LOGL_02_VAO::OnUpdate() {
	// 4) Use shader program, bind VAO and draw the triangle
	glUseProgram(m_shaderProgram);
	glBindVertexArray(m_vaoA);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(m_yellowShaderProgram);
	glBindVertexArray(m_vaoB);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}