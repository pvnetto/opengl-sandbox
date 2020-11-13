#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void OnWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void HandleWindowInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {

    int width = 800, height = 600;

    glfwInit();

    // OpenGL version 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Specifies that OpenGL is using Core profile, instead of Immediate
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initializes window
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Sandbox", NULL, NULL);
    if(!window) {
        std::cout << "Failed to create an OpenGL window :(\n";
        glfwTerminate();
        return -1;
    }

    // Makes context of the window the current OpenGL context, so any calls to OpenGL will be applied to this context
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, OnWindowResize);

    // Loads Glad by passing the extension loader function as parameter.
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize Glad :(\n";
        return -1;
    }

    // Before making any calls to OpenGL, the viewport must be initialized
    glViewport(0, 0, width, height);

    while(!glfwWindowShouldClose(window)) {
        // Processes inputs
        HandleWindowInput(window);

        // Rendering
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);   // Fills the color buffer with clearColor

        // Swaps buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}