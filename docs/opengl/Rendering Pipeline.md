# Rendering

## Rendering pipeline

**Normalized Device Coordinates (NDC)**:


## Basic renderer

0. Initialize window with GLFW or some other windowing API
- Just look at a boilerplate

1. Initialize renderer


2. Create/bind VAO
- **glCreateVertexArrays**

3. Create buffer objects (VBO)
- **glCreateBuffers**

4. Bind a buffer object to GL_ARRAY_BUFFER and feed data to it 
- **glBindBuffer**
- **glBufferData** or **glBufferStorage** or any **named** versions

5. Define buffer layout, so it can be accessed from shaders
- **glVertexAttribPointer**: Defines how to read data from buffer
- **glEnableVertexAttribArray**: Enables a previously defined attrib pointer. **This is actually for the VAO. [Refer to this](https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object)**
- This is like adding a pipe that goes from the buffer to a shader

6. Define shader program with AT LEAST vertex and fragment shaders


7. Clear framebuffer, Bind VAO and draw it!!
- **glClearBufferfv**: Clears the specified channel for current framebuffer
	- Channel options are: GL_COLOR, GL_DEPTH and GL_STENCIL
- **glBindVertexArray**: Binds VAO to context
- **glDrawElements**: Used when VAO has a vertex buffer bound to GL_ELEMENT_ARRAY_BUFFER with indices
	- OR **glDrawArrays** otherwise 