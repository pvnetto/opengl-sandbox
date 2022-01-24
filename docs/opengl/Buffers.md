# Buffers

## What the fuck are OpenGL buffers?
- Buffers are **general purpose blocks of data** that **store data contiguously**, like arrays;
- They must be **bound to a target**, so that OpenGL knows how to use them;
- Esse processo é todo **feito pela CPU**, então o ideal é passar sempre o maior número de vértices possível de uma só vez
    - Isso é feito para evitar um gargalo, pois a CPU é muito mais lenta que a GPU

### Buffer methods

Lifecycle:
- **glCreateBuffers**
- **glBindBuffer**
- **glDeleteBuffer**
- **glIsA_**


Storage:
- **glBufferData**: Mutable storage. Each time you call it, previously allocated data is erased and a new memory is allocated.
- **glBufferStorage**: Immutable storage. Read/write is permitted, but allocated size is constant
	- **glBufferSubData**: Writes to storage


### Targets
- Tells OpenGL how you intend to use a buffer
- The most common targets are:
	- GL_ARRAY_BUFFER: Defines vertex data and layouts. This target is accessible from shaders;
	- GL_ELEMENT_ARRAY_BUFFER: Defines indices for vertices. This target is used by draw function to draw vertices properly
- Some OpenGL functions only make sense with certain buffer targets
	- ex: glVertexAttribPointer is used by GL_ARRAY_BUFFER
	- ex: glDrawElements can only be called if GL_ELEMENT_ARRAY_BUFFER was bound


### Vertex buffers

```cpp
float vertexData[] = {
    // vertices         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

unsigned int vbo;
glCreateBuffers(1, &vbo);
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);
```


### Element Buffer Objects
- Um dos problemas de desenhar itens usando apenas VBOs é que os vértices precisam ser declarados em ordem para cada triângulo
- Isso significa que mesmo que dois objetos tenham vértices iguais, eles não podem ser reaproveitados

```cpp
// Perceba que os triângulos compartilham seus vértices 2 e 3
float vertices[] = {
    // Triângulo 1
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,

    // Triângulo 2
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};
```

- Para resolver esse problema, é preciso declarar um EBO (Element Buffer Object)
    - Ele possui índices para os vértices de um objeto
    - Esses índices dizem ao OpenGL quais vértices de um VBO devem ser usados para desenhar um triângulo
    - Isso significa que precisamos apenas declarar os vértices únicos e os indices de cada triângulo

```cpp
float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,        // triangle 1
    1, 2, 3         // triangle 2
};
```

- Eles são declarados iguai a qualquer buffer
    - Assim como as VBOs, os VEOs ficam associados ao VAO atual

```cpp
unsigned int ebo;
glGenBuffers(1, &ebo);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

- Para desenhar elementos a partir de um EBO, utiliza-se a função `glDrawElements`
    - O EBO é passado para o VAO, então só é preciso dar bind nele se ele não tiver em um VAO 

```cpp
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```


### Mapping
- Storing data to buffers will create a copy, so if you want to read/write from it, it might be a good idea to map it;
- **glMapNamedBuffer**: Maps the buffer to a memory location on the client, instead of copying the data back and fourth;

**TODO: Add mapping example**


### References
- Ref: https://stackoverflow.com/questions/27810542/what-is-the-difference-between-glbufferstorage-and-glbufferdata