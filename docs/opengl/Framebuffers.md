# Framebuffers


**Questions**
- What happens if I enable depth/stencil but don't specify an attachment for them?
**Questions**


- After fragment shader is executed, the result is written to a **color buffer**;
- This buffer represents each pixel that will be shown on the screen, so it usually has the dimensions as the screen;
	- For an application running on 1920x1080, the buffer should usually have 2,073,600 entries;
- Each entry on the buffer contains information about its corresponding pixel;
- You can have more than one color buffer at a time;
- Color buffers are only one of the types of buffers we can associate to a pixel though, as there are also:
	- Depth buffers;
	- Stencil buffers;
- A **framebuffer** is a structure that contains **a combination of one or more of these buffers**;


**Window-system-provided vs. Application-provided**:


## Usage

**Creating/Binding**:
- **glCreateFramebuffers**: Creates framebuffer objects;
- **glDeleteFramebuffers**: Deletes framebuffer objects;
- **glBindFramebuffer**: Binds framebuffer to a target operation;
	- **GL_READ_FRAMEBUFFER**: Read-only target. Reading will return values from framebuffer bound to this target;
	- **GL_DRAW_FRAMEBUFFER**: Write-only target. Drawing will update values of framebuffer bound to this target;
	- **GL_FRAMEBUFFER**: Read/write target. Both reading and drawing will happen on framebuffer bound to this target;

- **glNamedFramebufferParameteri**: Sets parameters for framebuffer when it **doesn't have attachments**;
	- Otherwise, parameters are determined by the attachments;
	- **GL_FRAMEBUFFER_DEFAULT_WIDTH**: Framebuffer width;
	- **GL_FRAMEBUFFER_DEFAULT_HEIGHT**: Framebuffer height;
	- [And some more](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glFramebufferParameteri.xhtml).


**Invalidating**:


**Blitting**:
- Blitting is the process of copying bits from one buffer to another;
- One important use case for blitting is **offline-rendering** with framebuffers;
	- In that case, we might want to **blit the final result** into the window-system framebuffer;

- **glBlitNamedFramebuffer**: Blits a specified area from **src** framebuffer to an area of **dest** framebuffer;
	- **src** must be bound to **GL_READ_FRAMEBUFFER**;
	- **dest** must be bound to **GL_DRAW_FRAMEBUFFER**;


```cpp

```


## Attachments


**Completeness**:



## Renderbuffers


**Usage**:
- **glCreateRenderbuffers**: Creates a renderbuffer object;
- **glNamedRenderbufferStorage**: Specifies storage for a renderbuffer;
	- internalformat must match the type of attachment the buffer will serve as;
	- **GL_RGBA**: Indicates that the buffer will be used as a **color** attachment;
	- **GL_DEPTH_COMPONENT**: Indicates that the buffer will be used as a **depth** attachment;
	- **GL_STENCIL_INDEX**: Indicates that the buffer will be used as a **stencil** attachment;
	- **GL_DEPTH_STENCIL**: Allows the buffer to be attached as either **depth**, **stencil** or **both** attachments combined;
- **glNamedFramebufferRenderbuffer**:

**What to use for attachments: Renderbuffers or Textures**:
- **Renderbuffers**: Basically buffers we can put render info into;
	- Use them when you know you **won't need to use the attachment as a texture**;
	- There are **some performance gains** in using renderbuffers, but they're **not big**;
- **Textures**: Can do the same as renderbuffers and still serve as a texture;
	- When in doubt, just use textures, there's nothing to lose from using them;


## Buffers

### Color Buffers
- Fragment shaders should return normalized colors ([0, 1] range);


### Depth Buffers


### Stencil Buffers


## Masks
- Enables/disables writing to specific buffers;
- **glColorMask**: Enables/disables writing to specific color buffer channels;
- **glStencilMask**: Sets a bitmask that is applied to stencil buffer on write;
- **glDepthMask**: Enables/disables writing to depth buffer;


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


### 2) Multisample fragment operations
- Multisampling is enabled by default and calculates pixel coverage value for fragments;
	- This default calculation **doesn't depend on alpha**;
- You can **glEnable** one or more modes so that multisampling also uses alpha to calculate coverage;

**Usage**:
- **GL_SAMPLE_ALPHA_TO_COVERAGE**: Uses alpha value to calculate coverage. This depends on OpenGL implementation;
- **GL_SAMPLE_ALPHA_TO_ONE**: Sets alpha to 1.0 during multisampling;

**TODO: Do some research on multisampling and then come back to this**



### 3) Stencil
- Stencil testing only takes place **if stencil buffer is enabled**;
- It compares the value in buffer with a reference value using a comparison function;
- Depending on the result of this comparison, the fragment can be either discarded or accepted;
	- This can be controlled by **glStencilOp**;


**Usage**:
- Enabled by **GL_STENCIL_TEST**;
- **glStencilFunc**: Sets the comparison function, a reference value and a mask that is ANDed to the buffer and reference values;
	- **GL_NEVER**: Never passes the test;
	- **GL_ALWAYS**: Always passes the test;
	- **GL_EQUAL**: Passes the test if buffer value equals reference value;
	- [And many more](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glStencilFunc.xhtml);
- **glStencilOp**: Controls **what is written to stencil buffer** when stencil test fails, depth test fails or both tests pass;
	- **GL_KEEP**: Keeps the current value in buffer;
	- **GL_ZERO**: Sets value to 0;
	- **GL_REPLACE**: Sets value to reference, which is defined by **glStencilFunc**;
	- [And many more](https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glStencilOp.xml);


```cpp
/* Initialization: Enables stencil testing and resets all buffer values 0 */
glEnable(GL_STENCIL_TEST);
glStencilMask(0xFF);
glClear(GL_STENCIL_BUFFER_BIT);


/* Writing */
// The value that is written to the buffer is determined by glStencilOp.
// In this case we'll keep the value when either tests fail and write 1 when both tests pass,
// so the buffer is only modified when both tests pass.
glStencilOp(GL_KEEP, GL_KEEP, GL_ONE);

// Determines that stencil testing will always pass, so:
// - Fragment can't be discarded by stencil testing;
// - Due to how glStencilOp was set up, we'll write 1 to stencil buffer whenever depth also passes.
glStencilFunc(GL_ALWAYS, 1, 0xFF);

// Runs the fragment shader, thus writes to the stencil buffer if thingy passes depth test
DrawThingy();
```

- This is kinda confusing because:
	- Writing to the stencil buffer depends on whether it passes or fails stencil testing
	- Passing/failing depends on values that are already written to the buffer, so it's possible to layer stenciling;


**Use Cases**:
- Masking;
- Object outlining;

**References**:
- https://learnopengl.com/Advanced-OpenGL/Stencil-testing


**TODO: Make shitloads of example to make it obvious how stencil buffer works**
**TODO: Outlining example**
**TODO: Wonderbox death transition example**


### 4) Depth

- Aka z-testing;
- For each pixel on the screen, the depth buffer **keeps track of the distance between the pixel and the viewpoint**;
- Depth testing passes or discards fragments depending on the value stored in depth buffer;
- When a fragment **passes** the depth testing, its **depth value is stored** in the **depth buffer**;


**Usage**:
- Enabled by **GL_DEPTH_TEST**;
- **glDepthFunc**: Sets up the comparison function for depth testing;
	- **GL_LESS** (default): Fragments with depth value **smaller than** current value in buffer will pass the test;


**Use Cases**:
- Mostly used to discard fragments of hidden surfaces;


### 5) Blending

- Blending uses a fragment's alpha to combine its color with the color in the color buffer;
- It only takes place if a fragment **passes all previous tests**;

- **Source**: Color of source fragment;
- **Destination**: Color in the color buffer (or destination pixel);

- Colors are blended by:
	- 1. Applying a **blending factor** to each channel of source and destination;
	- 2. Using a **blending equation** to combine each channel. By default, the equation just sums each channel;


**Blending factors**:
- Control the contribution of each color channel in source/destination before blending them together;
- There are separate blending factors for **source** and **destination**;
- Blending factors are **multiplied** to their respective source and destination colors;


```cpp
/* Defines how color channels are combined after blending factor is applied. Sums by default */
vec4 BlendingEquation();

/* Suppose those are given as input */
vec4 sourceColor, destColor;

/* Blending factors are defined with one value for each channel  */
vec4 sourceFactor { 1, 1, 1, 1 };
vec4 destFactor { 1, 1, 1, 1 };

/* Source/destination colors are multiplied by their factors */
vec4 finalColor = BlendingEquation(sourceFactor * sourceColor, destFactor * destColor);
```


**Usage**:
- Enabled by **GL_BLEND**;
- **glBlendFunc**: Determines how **blending factors** for source and destination are computed;
	- **GL_ONE**: (1, 1, 1, 1)
	- **GL_ZERO**: (0, 0, 0, 0)
	- **GL_ONE_MINUS_SRC_ALPHA**: (1, 1, 1, 1) - (src_alpha, src_alpha, src_alpha, src_alpha)
	- [And many more](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glBlendFunc.xhtml);
	- Defaults to: GL_ONE, GL_ZERO
- **glBlendEquation**: Selects an **equation to blend source/destination colors** after factors are applied;
	- **GL_FUNC_ADD**: src + dest
	- **GL_FUNC_MIN**: min(src, dest)
	- Defaults to: GL_FUNC_ADD


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
