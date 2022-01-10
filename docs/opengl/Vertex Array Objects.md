# Vertex Array Objects

- A VAO is an object that **stores all the state** needed to supply vertex data;
- When you bind a VAO to the OpenGL context:
	- All subsequent **buffer object changes** are **stored** in its state;
	- All state **already bound** to that VAO is **added** to the **OpenGL context**;
- VAOs are useful so you **don't have to setup your buffers every frame**;


**Caveats**:
- One important exception in VAOs is that **binding a VBO to GL_ARRAY_BUFFER won't modify the its state**. The state will only be **modified when defining vertex attribute layouts**;
- VAOs **do NOT store operations**, they're not like macros. So if you: Bind VAO > Bind BO > Unbind VAO > Modify BO, the changes will still show up in the VAO;



### Methods

Lifecycle:
- **glCreateVertexArrays**
- **glBindVertexArray**
- **glDeleteVertexArray**


Vertex layout specifiers:
- **glVertexAttribPointer**: 
- **glEnableVertexAttribArray**


Vertex layout specifiers (DSA versions):
- **glVertexArrayVertexBuffer**: Binds a VBO to a VAO state;
- **glVertexArrayAttribFormat**: Describes attribute layout for a VBO bound to a VAO; 
- **glEnableVertexArrayAttrib**: Enables an attribute layout;



**Example:**
```cpp
/* This should work fine, as long as we define vertex attributes after binding the VAO. */

unsigned int vao, vbo;

glCreateVertexArrays(1, &vao);
glCreateBuffers(1, &vbo);

// Binds VBO to GL_ARRAY_BUFFER before binding VAO
glBindBuffer(vbo, GL_ARRAY_BUFFER);
glBufferData(...);

// Binds VAO
glBindVertexArray(vao);

// Defines attributes
glVertexAttribPointer(...);
glEnableVertexAttribArray(...);
```

```cpp
/* This would NOT work. All buffers must be bound while the VAO is bound, except for GL_ARRAY_BUFFER. */
unsigned int vao, ebo;

glCreateVertexArrays(1, &vao);
glCreateBuffers(1, &ebo);

glBindBuffer(ebo, GL_ELEMENT_ARRAY_BUFFER);
glBindVertexArray(vao);
```


### References
- https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Array_Object
