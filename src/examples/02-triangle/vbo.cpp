#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

void OnWindowResize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void HandleWindowInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {

	int width = 800, height = 600;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL Sandbox", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create an OpenGL window :(\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnWindowResize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize Glad :(\n";
		return -1;
	}

	glViewport(0, 0, width, height);

	// 0) Declares VAO and VBOs + attributes
	
	// Vertices are drawn counter-clockwise when using glDrawArrays 
	float vertices[] = {
	    -0.5f, -0.5f, 0.0f, // left
	    0.5f, -0.5f, 0.0f,  // right
	    0.0f, 0.5f, 0.0f    // top
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// Binds VAO, so all subsequent buffers are bound to it
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Uses static draw because this is only set once and drawn many times

	// Attributes are declared on slot 0, so the shader should use layout = 0 to find them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
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
		return -1;
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
		return -1;
	}

	// 3) Declares shader program
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// (OPTIONAL) Check for program errors
	int programSuccess;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);

	if (!programSuccess) {
		char info[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, info);
		std::cout << "Failed to link shader program: " << info << "\n";
		return -1;
	}

	// (OPTIONAL) Deletes all shaders included in shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window)) {
		// === Processes inputs ===
		HandleWindowInput(window);

		// === Rendering ===
		glClearColor(0.5f, 0.2f, 0.1f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// 4) Use shader program, bind VAO and draw the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// === Swaps buffers ===
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}