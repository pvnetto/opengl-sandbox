# Framebuffers

## Fragment Tests

- After a fragment is processed by the fragment shader, we do several tests;
- Those tests check whether the pixel should be **written to the framebuffer or discarded**;
- When a fragment is discarded prior to them, they're not executed;
- **glEnable**: Enables/disables a test; 


### 1) Scissor
- Scissor testing discards all fragments inside the **scissor box**;
- The **scissor box** is a rectangular area inside the window;

**Usage**:
- Enabled/disabled with **GL_SCISSOR_TEST**;
- **glScissor**: Defines the scissor box;


### 6) Logical operations

**This is mostly used by bit-blip machines, so we're not going to dive too deep**

- Applies a **boolean operation** between source and destination values before writing to color buffer;

**Usage**:
- Enabled by **GL_COLOR_LOGIC_OP**;
- **glLogicOp**: Chooses a boolean operation to apply to source/destination values;
	- **GL_CLEAR**: 0;
	- **GL_COPY**: src;
	- **GL_NOOP**: dest;
	- **GL_AND**: src & dest;


## Advanced

### Writing to multiple renderbuffers/textures

- You might have noticed that it's possible to have **multiple color attachments** on the same framebuffer;
	- You can also have multiple stencil, depth attachments too, though less commonly used;
- By doing having multiple attachments, you allow your shader **write to multiple renderbuffers at once**;

**Usage**:

- 1) Setup your FBO as usual;
- 2) Bind a renderbuffer/texture to multiple attachments of the same type;
- 3) In the case of color attachments, specify the index of your attachment by either:

a. Using **layout (location = N)** in your GLSL shader;
```glsl
// Writes to color attachment 0
layout (location = 0) out vec4 fragColor;

// Writes to color attachment 1
layout (location = 1) out vec4 fragNormal;

void main() {
	...
}
```

b. Using **glBindFragDataLocation** to specify the attachment from the application;
- This function will bind output variables to attachment indices at **shader linking-time**;
```cpp
// Equivalent to previous example
glBindFragDataLocation(programID, 0, "fragColor");
glBindFragDataLocation(programID, 1, "fragNormal");
```
