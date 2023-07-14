## Instanced Rendering

- Executes the same drawing command multiple times in a row;
- Each drawing is called an **instance**, and receives an ID through **gl_InstanceID** on the vertex shader;
- Instancing allows you to draw the **same mesh** multiple times with **different attributes** each;
	- This means you **only have to specify vertices once**, avoiding repeated data;
- Instanced drawing is done using the **instanced** variation of existing draw functions;
	- **glDrawElementsInstanced**
	- **glDrawArraysInstanced**


```cpp
```

### Instanced arrays

- Instanced arrays are an alternative to uniforms when drawing instanced primitives;
- They're useful for sending **attributes that might change for each instance**;
	- ex: Transforms, colors are typical attributes that could be different for each instance;
- We use VBOs to store those attributes, and sample them **per-instance**, instead of per-vertex;

**Implementation**:
- **glVertexAttribDivisor**: Specifies how many instances to draw before sampling the next value in buffer;
	- ex: 1 = each instance will sample the next value from the buffer;
	- ex: 2 = a new value is sample every 2 instances, so [0, 1] would have the same value;

```cpp

```

**Why not just use uniforms?**:
- There's only 1 draw call, so if we used uniforms, we would have the same value for all instances;
- An array of uniforms indexed with gl_InstanceID would work, but we could reach the limit of uniforms per shader;