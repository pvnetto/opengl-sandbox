# Drawing

## Primitives

- Fragment shaders are run on a per-pixel basis, and not per-primitive; 
	- ex: For a single point that covers the entire screen, the vertex shader would run for each pixel on the screen;


### Points
- Number of vertices: 1;
- **glPointWidth**: Determines the size of a point;


### Lines
- Number of vertices: 2;
- Loops:
- Strips:
- **glLineWidth**:


### Triangles
- Number of vertices: 3;
- Strips:
- Fans:


## Draw methods

### Indexed
glDrawElements and all its variations are indexed.

### Non-indexed
glDrawArrays and all its variations are non-indexed.