# Writing a basic renderer

0. Setup a **CMake** project with **OpenGL**, **glad** and **GLFW**;
1. Initialize a **window** and **OpenGL context** using GLFW;
2. Pass geometry data to **buffers** and **vertex array objects**;
3. Load **shaders** and **textures**;
4. **Enable a shader** and use a **draw command**, which will write to the window **framebuffer**;


## Linking OpenGL to your application
To link OpenGL to your application, you'll need to link the OpenGL DLL to your application and then use an **OpenGL loader**, which will **load OpenGL functions from your video card driver**. Examples of loaders are **[glad](https://github.com/Dav1dde/glad)** and **[glew](https://github.com/nigels-com/glew)**.

Here's an example on how you would link OpenGL to your C++ program using CMake:


```cmake
# ==== root ====
add_subdirectory(src)
add_subdirectory(vendor)
```

```cmake
# ==== root/src ====

add_executable(TargetName main.cpp)

# Linking the opengl32 target to your own target is enough for CMake, as it will automatically find the DLL inside the system folder
target_link_libraries(TargetName glad)
target_link_libraries(TargetName opengl32)

target_include_directories(TargetName
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include)
```

```cmake
# ==== root/vendor ====
add_subdirectory(glad)
```

```cmake
# ==== root/glad ====

# glad DOES NOT come with a CMake configuration, so you have to do it manually.

cmake_minimum_required(VERSION 3.10)

project(Glad VERSION 1.0 LANGUAGES C)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

set(GLAD_SOURCES src/glad.c)
set(GLAD_HEADERS_DIR "${PROJECT_SOURCE_DIR}/include")
set(GLAD_ALL_HEADERS "${GLAD_HEADERS_DIR}/glad/glad.h" "${GLAD_HEADERS_DIR}/KHR/khrplatform.h")

add_library(glad STATIC ${GLAD_SOURCES} ${GLAD_ALL_HEADERS})

target_include_directories(glad PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                                         "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
```


**Extensions**:
Extensions are functions **included by video card manufacturers** to OpenGL that **do not exist in the specification**. They're usually optimizations or helpers for features that are not supported by native OpenGL.

To load an **extension**, you can use the **wglGetProcAddress** function passing the name of the extension as a parameter:

```cpp
void *res = wglGetProcAddress("name");
```

Note: This function is usually wrapped inside your window provider (like GLFW), so **usually you won't use wglGetProcAddress** directly, but the function that GLFW provides.


## Windowing
In order to draw to the screen, you need to tell the OS to **create a window** and then **pass data** to it. This is accomplished using a **windowing API**, like **GLFW**, **Win32** and **SDL2**. For simplicity we'll go with GLFW.

As you'll see later, when you create a window, it **comes with a framebuffer** attached to it. When you're drawing, OpenGL **writes data to that framebuffer**, and behind the scenes GLFW will use it to **tell the OS to draw its contents**.

**Setup**:
Setting up GLFW with CMake is straightforward. It comes with a CMakeLists.txt, so you just have to clone it [from github](https://github.com/glfw/glfw) to your project. 

```cmake
# root/vendor
add_subdirectory(glfw)
```

```cmake
# root/src

target_link_libraries(TargetName glfw)
target_include_directories(TargetName
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include)
```


**Initializing GLFW and creating a window**:
Creating a window with OpenGL is as simple as calling **glfwInit** and **glfwCreateWindow**.

```cpp
glfwInit();
GLFWwindow* window = glfwCreateWindow(1920, 1080, "How to OpenGL", NULL, NULL);
glfwMakeContextCurrent(window);
```

The interesting part about this initialization process is **glfwMakeContextCurrent**, which **initializes the OpenGL context** using the window we've created. That's because GLFW doesn't only do window creation, it **also abstracts OpenGL context creation**. This means **you can also initialize the context manually**, without GLFW, but it would require more advanced OpenGL knowledge.


**Setting up an user-defined data pointer**:
Most of the times you'll want your window to have some state data associated to it. In GLFW, this is done by **defining a state struct**, **instantiating it** an then calling **glfwSetWindowUserPointer** to bind the instance to a window.

```cpp
struct WindowState {
    // ...
};

WindowState state;
glfwSetWindowUserPointer(window, &state);
```

**Setting up events**:
GLFW also allows you to bind input and window events. To do this, you have to call the proper callback to the event you're trying to bind.

```cpp
// Window resizing
glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int newWidth, int newHeight) {
    // ...
});

// Mouse clicking
glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods) {
    // ...
});
```

There are many more callbacks, which you can find in GLFWs docs and examples.


**Note**:
- GLFW only works with OpenGL and Vulkan, so if you intend to support other rendering APIs on your program, like Metal and DirectX, you should probably support other APIs.



## Initialization
Before starting to use OpenGL functions, it's useful to specify which OpenGL version your application is targetting and the mode you're using. Some **people prefer to target older OpenGL versions (like 3.3)** because it's more likely they're supported by the graphics card they'll run from. As for the mode, you'll almost always **use core-profile**.

You can specify those things using **glfwWindowHint**:

```cpp
// Specifies target OpenGL version. In this case, 4.5.
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

// Specifies OpenGL mode. In this case, core-profile.
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```


## Direct State Access (DSA)
- Allows you to **use handles (a.k.a names) to access objects** directly, instead of having to bind them before accessing;
- Pretty much every gl function that **manipulates an object** will have a **DSA version** with the **__named__** preffix;
- ex: **glBufferData** is equivalent to **glNamedBufferData**