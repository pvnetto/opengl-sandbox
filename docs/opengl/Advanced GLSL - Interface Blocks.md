# Advanced GLSL

## Interface Blocks

When you have shader variables that you want to **share between with the application or between different shader stages**, you'll probably have to use **interface blocks**.

They come in the form of **uniform**, **buffer** and **in/out** blocks, but every one of those types share a similar declaration form, and each of them will have their own specifics that are added on top of this form.

```glsl
/* Declares an uniform block.
* 'BlockName' is its interface name.
* 'varName' is its variable name, and used to access its fields. This can be omitted.
*/

uniform BlockName {
	vec4 varA;
	bool varB;
} varName;		// Variables access works like 'varName.varA'. varName can be omitted.
```


## Uniform Blocks

- Uniform blocks are declared using the **uniform** keyword;
- Opaque types, like samplers, images etc are **NOT allowed**;


```glsl
uniform Matrices {
	mat4 ModelView;
	mat4 Projection;
	mat4 ColorKernel;
};
```

**Beware**:
- Variables inside uniform blocks are **not scoped**. You can't have 2 variables with the same name inside different uniform blocks;


### Uniform Block Layout

- Block/members layout are specified using the **layout** keyword and multiple qualifiers;
- Multiple qualifiers are comma-separated inside a layout keyword;


**Qualifiers**:

- **binding = N**: Specifies buffer's binding point (?);
- **packed**: Block should minimize its memory use;
- **shared**: Uniform block is shared through multiple programs;
- **std140**: Uniform block should use standard layout;
- **std430**: Buffer block should use standard layout;
- **row_major**: Uniform block matrices are forced into row major ordering;
- **column_major**: Uniform block matrices are forced into column major ordering;
- **offset = N**: Member is forced to be located at byte offset N in buffer;
- **align = N**: Member offset is forced to round up to multiple of N;

```glsl
layout (shared, row_major) uniform MyBlock {
	float number;
	layout (offset = 32) vec4 color;
};
```


**Writing to uniform blocks**:

Writing data to uniform blocks is done through a vertex buffer object. To do so, you need to:

1. Create/bind the VBO to the **GL_UNIFORM_BUFFER** target;
2. Get the uniform block index;
3. Get the block size and associate the VBO with the block;


```cpp
/* Gets block info */
int blockIndex = glGetUniformBlockIndex(programID, "BlockName");

int blockSize;
glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);


/* Gets block members' info */
constexpr int numUniforms = 4;
constexpr char* uniformNames[numUniforms] = { ... };

unsigned int indices[numUniforms];
int sizes[numUniforms];
int offsets[numUniforms];
int types[numUniforms];

glGetUniformIndices(program, numUniforms, names, indices);
glGetActiveUniformsiv(program, numUniforms, indices, GL_UNIFORM_OFFSET, offsets);
glGetActiveUniformsiv(program, numUniforms, indices, GL_UNIFORM_SIZE, sizes);
glGetActiveUniformsiv(program, numUniforms, indices, GL_UNIFORM_TYPE, types);


/* Creates buffer data */
void* buffer = malloc(blockSize);
// Use 'memcpy' to copy values to appropriate buffer locations here, using indices, sizes, offsets


/* Creates uniform buffer with previously created data */
int uniformVBO;
glCreateBuffers(1, &uniformVBO);
glBufferData(..., buffer, ...);
glBindBuffer(uniformVBO, GL_UNIFORM_BUFFER);


/* Binds uniform buffer to block */
// We use glBindBufferBase when the entire buffer is used for the same block
glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, uniformVBO);
```

Beware:
- Shader compiling/linking might eliminate some variables from the block, so uniform buffer size can't be hard-coded;


## Buffer Blocks

A lot similar to uniform blocks, except:
- Writing from the shader is allowed;
- VBO must be bound to **GL_SHADER_STORAGE_BUFFER** target;


## In/Out Blocks

Those are used to organize input/output data from different shader stages into blocks. They also use **layout (location = N)** syntax, and are pretty much syntatic sugar.

```glsl
in Lighting {
	layout (location = 0) vec3 normal;
	layout (location = 1) vec2 uv;
};

out Lighting {
	vec3 normal;
	vec2 uv;	
};
```

One interesting thing about those blocks is that you can specify the **component** when fetching multiple values from the same buffer location. So you could do:

```glsl
in Values {
	layout (location = 0, component = 0) vec2 offset;
	layout (location = 0, component = 2) vec2 uv;	
};

// Instead of

in Values {
	layout (location = 0) vec4 combined;
};

vec2 offset = combined.xy;
vec2 uv = combined.zw;
```