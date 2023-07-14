# OpenGL

## What is OpenGL?
OpenGL is considered a graphics API, but it's actually a **specification for a graphics manipulation API**. What this means is that the Khronos group is responsible for maintaining the specification, releasing new versions and fixes, while **video card manufacturers implement the API**. It's similar to C++ in the sense that the C++ committee maintains the specification and compilers implement it;

OpenGL is usually **implemented in C**, but there are bindings for other languages;


### Where can I find OpenGL on my machine?
OpenGL is implemented by video card manufacturers, so it comes **inside the video card driver**. Since all motherboards have built-in video cards, you'll always find OpenGL on modern CPUs as a **dynamic linking library (DLL) inside the system folder**. Manufacturers implement specific versions of OpenGL, so when you use **modern OpenGL** functions, there's a chance it **won't work on older video cards**;

On Windows, you can find it in **/drive:/WINDOWS/System32/opengl32.dll**;


### Other APIs

**OpenGL ES**:
Multiplatform API for **embedded systems** (hence the ES), like smartphones, consoles and vehicles. It implements a **subset of the specification**, but also comes with some extra features;

**WebGL**:
Cross-platform API for 3D graphics on **web**. WebGL looks very similar to OpenGL ES, but it's **implemented in JavaScript**, so there are some differences;


## OpenGL API basics

OpenGL works like a huge **state machine**, and having this in your mental image will help you understanding it. The **current state** is stored in what OpenGL calls the **context**, so when we call an OpenGL function we're either **modifying or using data stored in the context**. 

So you can summarize OpenGL usage in two steps:
1. Using functions to modify the state, usually passing data from the CPU to OpenGL;
2. Using functions to consume the state, thus drawing something to the screen;


### Main concepts

- **Context**: Contains OpenGL's current state;

- **Object**: Collection of data that represent a portion of the OpenGL state;
    - You can think about them as structs, even though they're not;
    - An object may for example contain configurations of a window, like size, which colors are supported etc;
    - Some examples of objects are buffers, vertex arrays, textures, samplers etc

**Core-profile vs Immediate-mode**:
OpenGL can be used in two different ways, so when you're looking into older OpenGL code you'll stumble upon things written in **immediate-mode**, also known as fixed-function pipeline. This mode is a lot more **strict** and **hides a lot of implementation details**, so using it is easier, but it's inneficient and not flexible at all. For this reason, this mode was **deprecated in OpenGL 3.2**, so people don't use it anymore.

For modern OpenGL implementations there's the **core profile**, which forces you to use modern practices and as opposite to immediate-mode, is a lot more flexible and can be made efficient when used properly, but this makes it harder to use, as you have to truly understand how OpenGL works.
