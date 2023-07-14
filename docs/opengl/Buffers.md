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
	- `GL_ARRAY_BUFFER`: Defines vertex data and layouts. This target is accessible from shaders;
	- `GL_ELEMENT_ARRAY_BUFFER`: Defines indices for vertices. This target is used by draw function to draw vertices properly
- Some OpenGL functions only make sense with certain buffer targets
	- ex: glVertexAttribPointer is used by `GL_ARRAY_BUFFER`
	- ex: glDrawElements can only be called if `GL_ELEMENT_ARRAY_BUFFER` was bound


### Vertex buffers

- Vertex buffers are used to pass data down to vertex shaders;
- As you should already know by reading `Shaders.md`, you pass input data to shaders by declaring attributes with their respective layout indices;
- The way OpenGL handles this data flow (i.e., CPU > GPU > Shaders) is through vertex buffers;
- You can think of this process in a couple of parts:
    - 1. Creating the vertex buffer with `glCreateBuffers` or `glGenBuffers`;
    - 2. Binding it to the `GL_ARRAY_BUFFER` target;
        - This is essentially a global variable that stores the buffer handle for a given target;
    - 3. Filling this buffer with data, using `glBufferData`;
        - At this point, the data is moved from the CPU to the GPU;
    - 4. Enabling/setting up vertex attributes with `glEnableVertexAttribArray`/`glVertexAttribPointer`;
        - This essentially creates a reference on the VAO to the buffer bound to `GL_ARRAY_BUFFER`, with additional info so that shaders can query attributes from that buffer;
        - At this point, even if you unbind the buffer, it won't make a difference to the VAO, as the entire point of `glVertexAttribPointer` is to create that reference;
        - If you modify the buffer data though, the VAO will keep track of the new data, as it contains just a reference to the buffer;
- Check [here](https://webglfundamentals.org/webgl/lessons/resources/webgl-state-diagram.html?exampleId=draw-cube-on-cube#no-help) if you want to visualize the internal state for this;

> Note that the instant nature of `glVertexAttribPointer` implies that you can use different buffers for different attributes (bgfx does this with its streams), but it's more recommended to use a single buffer, as it would be simpler and require less buffer bindings, so potentially faster as well.
> 
> Check [here](https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object) for more. 

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

- If your version of OpenGL is greater than `4.3` then it probably supports the `ARB_vertex_attrib_binding` syntax;
- This allows you to use an alternative way to declare attributes, with a **clearer separation between buffers and attribute layouts**;
    - `glBindVertexBuffer`: Binds a vertex buffer to a binding point, while also declaring its stride and buffer offset;
    - `glVertexAttribFormat`: Specifies the format of an attribute by index, kinda like `glVertexAttribPointer` would, but without having to specify offset and stride;
    - `glVertexAttribBinding`: Binds an attribute to a binding point;
    - We still have to use `glEnableVertexAttribArray`;
- Note that this flow assumes you've already created a buffer and submitted its data;


```cpp
struct Vertex {
    Vec3 Position;
    Vec2 UV;
    Vec3 Normal;
};

glBindVertexBuffer(0, vbo, offset, stride);

// Formats attributes
glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position));
glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, UV));
glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal));

// Binds attributes to a binding point, so from now on, any vertex buffer bound to that binding point will inherit this format
glVertexAttribBinding(0, 0);
glVertexAttribBinding(1, 0);
glVertexAttribBinding(2, 0);

// Enables all attributes
glEnableVertexAttribArray(0);
glEnableVertexAttribArray(1);
glEnableVertexAttribArray(2);
```

- **Note**: Attribute layouts are set on a per-VAO basis.
    - So the vertex format that a buffer receives when `glBindVertexBuffer` is called depends on which VAO is bound;
    - Also, there's a limit of binding points per VAO defined by `GL_MAX_VERTEX_ATTRIB_BINDINGS` (usually 16);

- References:
    - https://www.khronos.org/opengl/wiki/Vertex_Specification#Separate_attribute_format 
    - https://stackoverflow.com/questions/37972229/glvertexattribpointer-and-glvertexattribformat-whats-the-difference 
    - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindVertexBuffer.xhtml
    - https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribFormat.xhtml

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
