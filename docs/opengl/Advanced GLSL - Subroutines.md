# Advanced GLSL

## Subroutines

- Subroutines avoid conditional branching when selecting between multiple functions with the same parameters;
- They allow you to select which function to use from the application;
- In order to use them, you must:

0. Define a subroutine signature (just like a prototype);
```glsl
subroutine vec4 LightingFunction(vec3);
```

1. Define a pool of subroutines;
```glsl
subroutine (LightingFunction) vec4 ambient(vec3 normal) { }
subroutine (LightingFunction) vec4 diffuse(vec3 normal) { }
```

2. Declare an uniform with the subroutine type;
```glsl
subroutine uniform LightingFunction lightingMode;
```

3. Select which subroutine to use from the application;
```cpp
int subroutineLocation = glGetSubroutineUniformLocation(program, GL_VERTEX_SHADER, "lightingMode");
int ambientIndex = glGetSubroutineIndex(program, GL_VERTEX_SHADER, "ambient");

int subroutineUniformsCount;
glGetIntegerv(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS, &subroutineUniformsCount);

/* Contains subroutine index for each subroutine uniform in the shader. In this case
 * there's only 1 uniform, but there could be multiple */
std::vector<int> indices;
indices.reserve(subroutineUniformsCount);
indices[subroutineLocation] = ambientIndex;

/* Initializes all subroutine uniforms at once. */
glUniformSubroutinesuiv(GL_VERTEX_SHADER, subroutineUniformsCount, indices.data());
```