# Textures

- **texel**: Texture element or texture pixel. It's obtained from memory and represents the texture color for a given fragment;
	- ex: Sampling a texture from an UV would yield a texel;

```cpp
int textureID;
glCreateTextures(GL_TEXTURE2D, 1, textureID);
glTextureStorage2D(...);
glTextureSubImage2D(...);

// ...create another texture...

// Binds program and binds each texture to a texture unit, so they get picked up by shader uniforms
glUseProgram(programID);
glBindTextureUnit(0, textureID);
glBindTextureUnit(1, anotherTextureID);
```

```glsl
// fragment shader

layout (binding = 0) uniform sampler2D tex1;
layout (binding = 1) uniform sampler2D tex2;

void main() {
	// ...
}
```


## Texture formats

**Internal format**:
- Refers to the format in which **OpenGL will store your data**;
- Format names follow the convention: `GL_<channels><size><type>_<operation>`

- ex: GL_RGBA8I
	- `<channels> 	= RGBA`: 	Each entry has Red, Green, Blue and Alpha channels;
	- `<size> 		= 8`: 		Each channel will have 8 bytes;
	- `<type>		= i`:		Each channel is represented by an integer;


**External format**:
- Refers to the format in which **OpenGL will read your data**;
- So when you read your image, this is the format it should be **stored in your memory**;
- Also follows a naming convention: `GL_<channels>_<(opt.) INTEGER>`
	- INTEGER is an optional suffix that indicates data should be read as **unnormalized integers**;
- External format is followed by a `type` parameter that describes type data in memory;
	- We need this parameter because external format doesn't specify type/size;
	- ex: GL_BYTE, GL_FLOAT, GL_UNSIGNED_INTEGER etc;

- ex: GL_RGBA_INTEGER
	- `<channels> 	= RGBA`: 	Each entry has Red, Green, Blue and Alpha channels;
	- `<INTEGER>`: 				Data should be read as unnormalized integers;


## Reading data from textures

- **glGetTextureImage**: This function very poorly design. It works by either:
	- a. Passing a pointer to the **pixels** parameter, which will receive all the data by copy;
	- b. Binding a buffer object to **GL_PIXEL_PACK_BUFFER**, and then calling glTextureImage using **pixels** as an offset;
		- This is a lot more performant, and will actually map the pixels instead of copying;


## Sampler objects
- Sampler objects determine rules on sampling specific textures;
- They work just like other OpenGL objects, with the exception that they need must be **bound to a texture unit**:
	- **glCreateSamplers**: Creates a sampler object;
	- **glBindSampler**: Binds a sampler object to a texture unit;
- All texture objects have a default internal sampler object, which you can alternatively use;


**Sample parameters**:
- **GL_TEXTURE_MIN_FILTER**: Determines how sampling will behave when a pixel occupies **more than one texel**;
- **GL_TEXTURE_MAG_FILTER**: Determines how sampling will behave when a pixel occupies **less than one texel**;
- **GL_TEXTURE_WRAP_S**: Sets wrapping mode for texture coordinate **s** (or x/u in UV coordinates);
- **GL_TEXTURE_WRAP_T**: Sets wrapping mode for texture coordinate **t** (or y/v in UV coordinates);
- [And many more](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glSamplerParameter.xhtml);


**Configuring sample parameters for a sampler object**:
- **glSampleParameter{i/f/v}{v}**: Sets a parameter for a named sampler object;
	- This uses DSA, so there's no need to bind the sampler;


**Configuring sample parameters for the default sampler object**:
- **glTextureParameter{i/f/v}{v}**: Same as glSampleParameter, but will access the default sampler for the texture instead;
	- Also uses DSA, so there's no need to bind the texture;


**LOD**:
- As mentioned, mipmaps can be used to implement LOD;
- LOD in OpenGL comes by default with mipmaps, all you have to do is configure some sampler parameters;
- Those parameters will **change how OpenGL calculates mipmap levels**;
	- For more, check Ch. 6, "Calculating the Mipmap Level" on OpenGL red book;

- **GL_TEXTURE_LOD_BIAS**: Adds a constant bias to mipmap level calculation;
- **GL_TEXTURE_MIN_LOD**: Defines minimum value for mipmap level calculation. Defaults to -1000;
- **GL_TEXTURE_MAX_LOD**: Defines maximum value for mipmap level calculation. Defaults to 1000;

- **GL_TEXTURE_BASE_LEVEL**: Defines the lowest mipmap level (highest resolution) that can be sampled;
	- Useful for **texture streaming** (loading textures from lowest to highest resolution);
	- Use this to make sure that the **base level** is the texture with the **highest resolution loaded so far**;
- **GL_TEXTURE_MAX_LEVEL**: Defines the highest mipmap level (lowest resolution) that can be sampled;


- It's also possible to **completely override OpenGL's mipmap level calculation** with the GLSL function **textureLod**;


## Texture compression

- There are two ways you can use compressed textures in OpenGL:

**a. Storing textures as compressed data**:
- For this method, we'll ask OpenGL to **store uncompressed textures** using a **compressed internal format**;
	- Original file is not compressed;
	- OpenGL internal data is compressed;
- OpenGL will automatically compress textures when you **specify a compressed internal format**;

- This is usually NOT a good method, because OpenGL's compression algorithms are slow;


**b. Storing compressed textures as regular data**:
- This method uses an **offline compression algorithm** to compress textures before using them;
- Textures must be compressed in an **external format** known by OpenGL;
- **glCompressedTextureSubImage<1D/2D/3D>**: Similar to **glTextureSubImage**, but stores compressed data;
	- This should only be called after specifying storage with **glTextureStorage<1D/2D/3D>**;


### Cube Maps

##### Creating a Cube Map

```cpp
// Generates a cube map texture object
unsigned int textureID;
glGenTextures(1, &textureID);
glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
```

##### Populating a Cube Map

A cube 6 faces, so for a cube map, we have to bind one texture for each one of them, that is we have to call glTexImage2D 6 times. OpenGL provides us one texture target for each face, namely:

- GL_TEXTURE_CUBE_MAP_POSITIVE_X
- GL_TEXTURE_CUBE_MAP_NEGATIVE_X
- GL_TEXTURE_CUBE_MAP_POSITIVE_Y
- GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
- GL_TEXTURE_CUBE_MAP_POSITIVE_Z
- GL_TEXTURE_CUBE_MAP_NEGATIVE_Z

And just like many other OpenGL enums, they're ordered, so you can start from GL_TEXTURE_CUBE_MAP_POSITIVE_X and increment it by an integer value.

```cpp
for(int i = 0; i < 6; i++) {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, ...);
}
```

##### Sampling a Cube Map

To sample a cube map from the fragment shader, you should use a `samplerCube`, instead of the usual `sampler2d`.

```cpp
// fragment shader
in vec3 textureCoordinate;
uniform samplerCube cubeTexture;

void main() {
	// Also uses the 'texture' method for sampling
	FragColor = texture(cubeTexture, textureCoordinate);
}
```