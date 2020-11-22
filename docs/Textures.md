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

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aUVCoord;

out vec3 vertexUV;

void main() {
    gl_Position = vec4(aPosition, 1.0f);
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

in vec3 vertexUV;

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
