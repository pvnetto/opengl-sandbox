# Textures

- Textures are images that serve the purpose of giving your objects color details, just like wallpapers would;
- **texel**: Texture element or texture pixel. It's obtained from memory and represents the texture color for a given fragment;
	- ex: Sampling a texture from an UV would yield a texel;


## Texture Types

- **GL_TEXTURE_2D**: Most common type of texture, it's just a 2D texture map. GLSL name: **sampler2d**;
- **GL_TEXTURE_CUBE**: Cube maps. GLSL name: **samplerCube**;
- **GL_TEXTURE_ARRAY**: Texture arrays are slices of textures. GLSL name: **samplerCubeArray**;


## Texture usage

- To make use of textures in OpenGL, you need to at least:
	- 1. Create a **texture object**;
	- 2. Bind the texture object to a **texture unit**;
	- 3. Include UVs in your vertex data;
	- 4. Add a `sampler**` uniform to your shader, with `layout (binding = n)` qualifier;
		- `n`: Texture unit of the texture; 
	- 5. Sample texel values from the sampler variable;

**Texture units**:
- Texture units allow you to **bind textures to the OpenGL context**, making them **accessible from shaders**;
- There's a limited number of texture units, which is defined by implementation;
- Textures bound to texture units can be accessed using a `sampler**` variable on your shader;


**Creating/binding/storing data**:
- Creating, binding and storing data to a texture object works kinda similar to buffers;

- **glCreateTextures**: Creates 1 or more textures of a given type, and returns a name for each of them;
- **glDeleteTextures**: Deletes 1 or more textures from an array of names;

- **glTextureStorage<1D/2D/3D>**: Creates immutable storage for a texture object;
	- Allocated size depends on **internalformat**, **width** and **height**;
	- Immutable means it can't be deallocated/resized, but read/write is enabled;
- **glTextureSubImage2D**: Writes to texture storage. This complements **glTextureStorage2D**;
	- Texture equivalent of **glBufferSubData**;

- **glBindTextureUnit**: Binds texture object to a **texture unit**; 
	- For this function, texture unit is given by a **GLuint**, and not an enum;
	- Units range from [0, max_units - 1], so binding to unit 0 will work;
	- Max number of units can be retrieved by **GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS**;
	- **You can only bind one texture of a specific target per texture unit**;
		 - ex: You can't create two GL_TEXTURE_2D textures and bind to texture unit 1;

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

- Alternatively, you can also set the texture unit for a texture through the application;

```glsl
// fragment shader

uniform sampler2D tex1;
uniform sampler2D tex2;

void main() {
	// ...
}
```

```cpp
// Sets texture unit for each sampler uniform
int loc1 = glGetUniformLocation(programID, "tex1");
int loc2 = glGetUniformLocation(programID, "tex2");

glUseProgram(programID);
glUniform1i(loc1, 0);
glUniform1i(loc2, 1);
```

**Non-DSA texture binding**:
- Alternatively, you'll see lots of code that uses the non-DSA version of texture binding, so it's important to know how it works;

- **glActiveTexture**: Activates a specific texture unit, so all subsequent bindings are applied to it;
	- Receives **GL_TEXTURE0/1/2/...** as parameter, instead of a GLuint;
- **glBindTexture**: Binds a texture object to a specific texture target in the active texture unit;
	- Receives a texture name and target (ex: GL_TEXTURE2D) as parameter;
	- Non-DSA texture objects don't have a predefined target, so they need to specify it when binding;

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE2D, textureID);
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


## Texture Filtering

- **Minification**:
- **Magnification**:

### Linear Filtering
- Replaces a sample with a linear approximation of two of its closest adjacent texels;
- This technique results in smoother samples when the texture is **magnified**;
- Enabled by setting sampler's **GL_TEXTURE_MAG_FILTER** to **GL_LINEAR**;
	- **GL_NEAREST** would just return the nearest sample instead, with no combination;


### Mipmaps
- Mipmaps are textures with **successively lower resolution** than the original version;
- They solve the issue of **undersampling**, which happens when the pixel/texel ratio is too low;
	- i.e: When one pixel can sample multiple texels;
	- This means they're used for **texture minification**;
- Mipmaps are usually generated until 1x1 resolution is reached;
- They work better when textures are **square-sized**, preferably by a **power of 2**;
	- ex: 256x256, 512x512, 1024x1024 etc;

**Use cases**:
- ex: Let's suppose you're using a high resolution texture to render a model;
	- When the model is nearby, there are a lot of pixels relative to texels, so it makes sense to use higher resolution textures;
	- As we get farther from the model, it gets smaller relative to the viewer but the texture doesn't change;
	- This causes the pixel/texel ratio to fall, so image artifacts will appear due to **undersampling**;
	- By using mipmaps, we can sample from a smaller texture everytime the pixel/texel ratio goes below a certain threshold, thus preventing undersampling;

- Mipmaps are useful for LOD implementation;

- **TODO: Add image**;


**Usage**:
- **glGenerateTextureMipmap**: Generates mipmaps for a texture;
	- Set **GL_TEXTURE_BASE_LEVEL** with **glTextureParameteri** to control the mipmap count for a texture;

- To enable mipmap usage, set your texture's **GL_TEXTURE_MIN_FILTER** to **GL_{A}_MIPMAP_{B}**, where:
	- **{A}** and **{B}** can be either **LINEAR** or **NEAREST**;
	- **{A}** controls how mipmap levels are constructed;
	- **{B}** controls how samples are constructed when choosing between mipmap levels;



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


## Advanced texture types


### 3D textures


### Texture Arrays


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

**Seamless sampling**:

# Texturas
- Texturas são imagens 2D que servem para dar cor a um objeto
- Pintar cada vértice não seria prático para modelos grandes, além de dar pouco controle ao resultado final
    - Por isso geralmente utiliza-se uma textura que é mapeada aos vértices através de coordenadas UV

## Texture sampling
- Texturas são definidas por coordenadas no intervalo **[0, 1]**
    - São definidas pelos eixos **(s, t)**, correspondentes a **(x, y)**
    - Começam no lado **inferior esquerdo**, na coordenada (0, 0)
    - Terminam no lado **superior direito**, na coordenada (1, 1)
- Texture sampling consiste em **mapear um vértice a um texel**
    - **Texel**: Abreviação para texture pixel
- Existem várias formas de fazer isso, cabe a nós dizer ao OpenGL qual forma usar

### Coordenadas UV
- O uso de Coordenadas UV é a forma mais comum de texture sampling
- Seus valores geralmente ficam no intervalo **[0, 1]**, mas é possível que passem disso
    - Não necessariamente seguem o mesmo padrão das texturas
- No OpenGL é possível especificar as coordenadas UV em um VBO

```
float vertexData[] = {
    // posição              // uv
    0.5f, 0.5f, 0.0f,      1.0f, 1.0f,
    ...
};
// Perceba que o stride tem tamanho 5 (3 floats de posição + 2 de uv)
// e o offset tem tamanho 3 (3 floats para posicao que vem antes dos 2 de uv)
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

## Wrapping
- As vezes tentamos samplear uma textura usando coordenadas fora do intervalo [0, 1]
- Para especificar o comportamento da textura nesses casos, temos o **texture wrapping** 
    - Repeat: Apenas repete a textura na magnitude que as coordenadas passam
        - Ex: 1.1 é mapeado para 0.1
        - Denotado no OpenGL por **GL_REPEAT**
    - Mirrored repeat: Semelhante ao repeat, mas da mirror a cada repetição
        - Denotado no OpenGL por **GL_MIRRORED_REPEAT**
    - Clamp: Arredonda coordenadas fora do intervalo. No OpenGL, isso pode ser feito de duas formas
        - Clamp to edge: Arredonda para o intervalo [0, 1]. Denotado por **GL_CLAMP_TO_EDGE**
        - Clamp to border: Coordenadas fora do intervalo recebem uma cor específica. Denotado por **GL_CLAMP_TO_BORDER** 
- O wrapping deve ser especificado para cada eixo da textura (s, t)

```
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED);
```

## Filtering
- Quando temos um modelo com muitos vértices e uma texture de baixa resolução, as cores podem ser comportar de forma não desejada
- Para mitigar esse problema, o OpenGL oferece duas formas de mapear vértices a texels
    - **Point**: Arredonda as coordenadas para o pixel mais próximo
        - Denotado no OpenGL por **GL_NEAREST**
    - **Linear**: Usa as coordenadas para fazer um lerp entre os pixels mais próximos, denotado por 
        - Pode ser **bilinear** ou **trilinear**
        - Denotado no OpenGL por **GL_LINEAR**
- Para especificar qual filtragem usar, basta usar a função `glTexParameteri`, que setta um parâmetro da textura

```
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

- O OpenGL permite especificar filtering diferente para texturas que sofrem downscale/upscale, utilizando os parâmetros:
    - `GL_TEXTURE_MIN_FILTER`: Texturas com **downscale**
    - `GL_TEXTURE_MAG_FILTER`: Texturas com **upscale**

## Mipmaps
- Existe também o problema de modelos com **poucos vértices/fragmentos** e **texturas de alta resolução**
    - Esse problema costuma se manifestar quando **objetos estão distantes da câmera**
    - Isso pode gerar artefatos estranhos e problemas de otimização (texturas desnecessariamente grandes)
- Para resolver esse problema utiliza-se **mipmaps**
- Mipmaps são apenas **downsamples de uma textura**, utilizados quando objetos geram poucos fragmentos 
- Eles podem ser gerados tanto **manualmente** quanto **automaticamente pelo OpenGL**
    - Gerá-los manualmente é muito trabalhoso
    - Geralmente isso é feito de forma automática com a função `glGenerateMipmaps`

```
glGenerateMipmaps(GL_TEXTURE_2D);   // Gera mipmaps para a textura ativa
```

- Para mipmaps, é possível especificar o filtering para cada nível de mipmap além do filtering de sampling para o nível escolhido
- As tags para filtering de mipmap seguem a mesma lógica das texturas
    - `NEAREST`: Arredonda o tamanho do pixel para o mipmap mais próximo
    - `LINEAR`: Interpola entre os dois mipmaps mais próximos do pixel
- Essas tags devem seguir o formato `GL_<FILTERING>_MIPMAP_<MIPMAP_FILTERING>`
    - Isto é, **filtering de textura seguido por filtering de mipmap**
    - `GL_NEAREST_MIPMAP_NEAREST`, `GL_LINEAR_MIPMAP_LINEAR` etc

```
glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
```

- Obs: Para mipmaps sempre utiliza-se `GL_TEXTURE_MIN_FILTER`, pois mipmaps são quase sempre downscales da textura padrão

## Carregando uma textura
- Texturas em OpenGL são **objetos**, então carregar uma textura é bem parecido com carregar qualquer outro objeto

```
unsigned int texture;
glGenTexture(1, &texture);
```

### stb

- Um dos problemas das texturas é que existem **vários formatos diferentes de imagem**
- Em geral utiliza-se uma **lib para carregar imagens**, como a **stb**

```
int width, height, numOfChannels;
unsigned char* data = stbi_load("my_texture.png", &width, &height, &numOfChannels, 0);
```

- Obs: As coordenadas das imagens da **stb são o inverso do OpenGL**, então é preciso **flippá-las antes de carregar**

```
stbi_set_flip_vertically_on_load(true);
```

- Após carregar as imagens, podemos passar seus dados para o objeto textura
- Passamos dados para texturas através da função `glTexImage2D`

```
glBindTexture(GL_TEXTURE_2D, texture);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
```

### Gerando mipmaps
- Após carregar os dados da textura, podemos gerar seus mipmaps

```
glGenerateMipmaps(GL_TEXTURE_2D);
```

- Se estivermos utilizando a stb, é uma boa prática desalocar os dados lidos da imagem

```
stbi_image_free(data);
```

### VBO
- Uma das formas de samplear uma textura é através do uso de coordenadas UV
- Essas coordenadas podem ser inclusas diretamente na VBO

```
float vertices[] = {
    // posições           // coordenadas UV
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};
```

### Vertex shader
- Assim como qualquer parâmetro da VBO, as coordenadas UV podem ser acessadas pelo vertex shader
- No entanto o sampling é feito pelo fragment shader, então o vertex shader irá apenas passar essas coordenadas como parâmetro out

```
// vertex shader
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aUVCoord;

out vec3 vertexUV;

void main() {
    gl_Position = vec4(inPosition, 1.0f);
    vertexUV = aUVCoord;
}
```

### Fragment shader
- Para que uma textura possa ser acessada via shader, é preciso criar um uniform do tipo **sampler2d**
    - Obs: Caso a texture seja 1d ou 3d, existem samplers para esses tipos
- As coordenadas UV são usadas pela função `texture` para samplear uma cor dessa uniform
- Por fim, a cor é passada para o output, e consequentemente desenhada na tela

```
// frag shader
#version 330 core

in vec2 vertexUV;

out vec4 fragmentColor;

uniform sampler2d tex;

void main() {
    fragmentColor = texture(tex, vertexUV);
}
```

##  Texture units
- Apesar da textura ser um uniform, nem sempre precisamos passá-la ao shader via CPU
- Isto acontece porque texturas são passadas via texture units
- Texture unit é a **localização de uma textura na memória**
    - Por padrão os sampler2d pegam a texture unit 0
- Elas possibilitam o uso de várias texturas no mesmo shader, desde que estejam em texture units diferentes
- Para isso, é preciso ativar uma location específica de texture unit e bindar uma textura a ela

```
glActiveTexture(TEXTURE_UNIT_0);
glBindTexture(GL_TEXTURE_2D, texture);

glActiveTexture(TEXTURE_UNIT_1);
glBindTexture(GL_TEXTURE_2D, otherTexture);
```

- Por **padrão**, a texture unit ativa é a de **índice 0**, assim como os sampler2d
- No entanto, através de uniforms podemos settar a texture unit que um sampler2d irá pegar

```
// fragment shader
uniform sampler2d texture;
uniform sampler2d anotherTexture;
```

```
// C++
int samplerLocation = glGetUniformLocation(shaderID, "otherTexture");
glUniformli(samplerLocation, 1);        // Setta a texture unit da textura para 1
```
