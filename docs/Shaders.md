# Shaders

- Shader são programas que rodam na GPU
- Cada shader representa um seção específica do pipeline gráfico
- Eles não podem comunicar diretamente entre si, exceto por seus inputs e outputs
    - Isso significa que geralmente um shader vai ter como input o output do shader anterior no pipeline gráfico
    - Ao final da execução, espera-se que o output de um shader seja passado como input para o próximo 

## GLSL
- GLSL é uma linguagem com **sintaxe semelhante a C** feita para computação gráfica, mais especificamente para shaders
- Isso significa que ela possui features que facilitam **manipulação de vetores e matrizes**

## Estrutura de um programa GLSL
- Um programa em GLSL costuma ter a seguinte estrutura:

```
#version <number> <type (core/immediate)>

in type <in_var_name_1>;
in type <in_var_name_n>;

out type <out_var_name_1>;

uniform type <uniform_name>;

void main() {
    // === Do graphics things ===
    ...
    // === Do graphics things ===

    <out_var_name_1> = <value>;    // Variáveis de saída sempre devem receber um valor
}
```

### Tipos
- Primitivos:
    - **int**
    - **uint**
    - **float**
    - **double**
    - **bool**

- Não-primitivos:
    - **vec**

#### Vectors
- Pode ter de 1 a 4 dimensões e qualquer um dos tipos primitivos
- Por padrão é um vetor de floats
- Quando assumem outros tipos, são declarados com um prefixo:
    - **bvec**: Vetor de booleanos
    - **ivec**: Vetor de inteiros
    - **uvec**: Vetor de inteiros unsigned
    - **dvec**: Vetor de doubles
- O sufixo é usado para denotar a quantidade de elementos:
    - **vec1**
    - **vec2**
    - **vec3**
    - **vec4** 

- Os elementos podem ser acessados por **x**, **y**, **z** e **w**
- Além disso, é possível acessar vários elementos por vez, e em qualquer ordem (swizzling)
- Esses valores podem ser passados como parâmetro, então é um syntatic sugar para esse tipo de operação

```
vec4 myVec = vec4(0.0f, 1.0f, 2.0f, 3.0f);

vec3 myVecXZ = vec3(myVec.xz, 1.0f);

vec3 myVecXXX = vec3(myVec.xxx);
```

### Ins/outs
- Shaders usam parâmetros in e out para compartilhar dados
- Estes são declarados usando as keywords **in** e **out**


```
in vec4 vertexColor;
out vec4 fragColor;
```

- Para parâmetros in em vertex shaders, é possível receber valores de vertex buffers usando as meta tags layout/location
    - **location**: Índice do vertex attrib pointer desejado

```
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
```


### Uniforms

- O que são uniforms?
- Como declarar uma uniform
- Como passar uma uniform de C++ para a instância do shader

- Quando a aplicação precisa enviar dados para o shader que não podem ser passados via vertex buffers, utilizamos uniforms
- Uniforms são **variáveis globais para o shader program**
    - Isso significa que são únicas por shader program e podem ser acessadas por qualquer shader do program
- O valor de um uniform não muda até que seja atualizado ou resettado
    - Ou seja, não é preciso settar a cada iteração
- Para declarar uma uniform em um shader, basta utilizar a keyword uniform

```
uniform vec4 ambientColor;
```

- Após declarar a uniform no shader, é preciso settar seu valor através do shader program

```
int ambientColorUniformLocation = glGetUniformLocation(shaderProgram, "ambientColor");      // Busca a localização da uniform

// É preciso usar o program antes de settar a uniform, para que a função saiba de qual program se trata
glUseProgram(shaderProgram);
glUniform4f(ambientColorUniformLocation, 0.0f, 1.0f, 0.0f, 1.0f);       // Passa a cor verde para a uniform
```

- Como OpenGL é feito em C, a função `glUniform` não tem overloads propriamente ditos
    - Por essa razão, os overloads são feitos através de sufixos, como por exemplo:
        - `glUniform3f`: Passa 3 floats para a uniform, que é o que um vec3 receberia
        - `glUniformui`: Passa um unsigned int para a uniform

## Vertex shaders
- OpenGL moderno exige que o desenvolvedor faça ao menos um vertex shader para desenhar na tela
- São o único tipo de shader que recebem input diretamente dos buffers, ao invés do output de outros shaders

```
#version 330 core	// Especifica versão do OpenGL

layout(location = 0) in vec3 aPos;	// Recebe dados do buffer no índice 0

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

- Esse tipo de shader deve devolver os vértices em NDC (normalized device coordinates)
    - Vértices com x, y e z fora do intervalo [-1.0, 1.0] são clippados após o primeiro passo de rasterização
    - O viewport do OpenGL transforma essas coordenadas em NDC para screen space, e então em fragmentos, que serão passados para o fragment shader

## Geometry shaders
- É o tipo de shader menos utilizado e o único que vem por padrão no OpenGL
- São responsáveis por distorcer a geometria, adicionando vértices, arestas etc

## Fragment shaders
- Assim como o vertex shader, também é obrigatório no OpenGL
- Fragment shaders recebem como input fragmentos
    - Um fragmento contém todos os dados necessários para desenhar um pixel (posição, cor etc)
- Calcula a cor final dos pixels, por esse motivo devem **sempre retornar um vec4 com uma cor**
- É onde os efeitos mais avançados geralmente são feitos

```
#version 330 core

out vec4 fragColor;	// Declara o output como um vec4 (cor)

void main() {
    fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);	// Retorna uma cor sólida para todos os pixels
}
```

- **Atributos passados via vertex buffer serão interpolados** antes de serem passados para um fragment shader
    - Isto porque os fragmentos passados pelo geometry shader são processados dos vértices, e a quantidade de fragmentos é sempre maior que a de vértices


## Compilando um shader

- Para compilar um shader, basta passá-lo em forma de string para a função glCreateShader do OpenGL
- A compilação de shaders é portanto feita de forma dinâmica, em tempo de execução

```
const char* vertexShaderSource = ˜// coding do shader aqui˜
unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // GL_FRAGMENT_SHADER caso seja um fragment shader

glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Vincula o código fonte do shader a um objeto shader
glCompileShader(vertexShader);
```

- Para verificar os logs de compilação do shader, basta rodar o código:

```
// Verifica se o shader compilou com sucesso
int success;
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

// Printa mensagens de erro caso o shader tenha erros de compilação
if(!success) {
    char infoLog[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << ˜Erro: ˜ << infoLog << ˜\n˜;
}
```

### Shader program

- Após compilar os shaders, é preciso linká-los a um shader program para que possam ser utilizados
- Ao linkar os shaders, o program automatiza a passagem da saída de um shader para a entrada do próximo

```
unsigned int shaderProgram;
shaderProgram = glCreateProgram();	// Cria o objeto do shaderProgram

glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);	// Linka os inputs e outputs dos shaders
```

- De forma semelhante a compilação, podemos verificar se a linkagem dos shaders funcionou:

```
Int sucess;
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

if(!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
}
```

- Para utilizar um shader no próximo draw call, basta utilizar a função glUseProgram
- Todas as chamadas de renderização ou de shaders após glUseProgram farão referência ao program utilizado

```
glUseProgram(shaderProgram);
```

- Após linkar os shaders, caso não queira utilizá-los em outros programas, é uma boa prática deletá-los

```
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```