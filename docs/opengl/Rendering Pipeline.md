# Rendering

## Rendering pipeline

- Em OpenGL tudo fica no espaço 3D, porém a janela é um array 2D de pixels coloridos
- O pipeline gráfico do OpenGL é dividido em duas partes:
    - 1) Transformar os objetos que estão no espaço de coordenadas 3D em pixels 2D
    - 2) Transformar essas coordenadas 2D resultantes em pixels coloridos

- Essas partes por sua vez possuem vários passos, e cada passo recebe como input o output do passo anterior
- São altamente especializados e podem ser executados em paralelo em programas na GPU
- Esses programas são chamados de **Shaders**, e a função deles é processar um passo do pipeline gráfico
- O pipeline pode ser resumido nos seguintes passos:
    - 1) Input: Recebe dados de vértices como input
    - 2) Vertex shader: Input = vértices, output = vértices modificados
    - 3) Shape assembly: Input = vértices, output = forma primitiva
    - 4) Geometry shader: Input = primitive, output = geometria completa
    - 5) Tests, blending: Input = geometria, output = fragmentos
    - 6) Fragment shader: Input = fragmentos, output = fragmentos modificados
    - 7) Rasterização


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


## Swap Buffers
Para que um renderer possa desenhar na tela, as informações de cor de cada pixel devem ser armazenadas em um buffer. Quando utilizamos apenas um buffer para isso, temos problemas de flicker, causado pelo tempo de resposta entre a troca do buffer anterior para o mais recente. No OpenGL isso é resolvido com o Swap Buffer, que utiliza dois buffers para desenhar na tela, sendo eles o front e back.

- Back: As saídas de comandos de render vão para esse buffer
- Front: Cor final da imagem que é mostrada na tela

O OpenGL por sua vez passa o conteúdo do back para o front buffer, então o front buffer nunca ficará vazio e o problema de flicker é resolvido.