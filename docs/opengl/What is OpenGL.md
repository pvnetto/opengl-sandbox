# OpenGL

- What is OpenGL?
- How to use OpenGL?


**Core vs Immediate**:


**Direct State Access (DSA)**:
- Allows you to use handles (called names in OpenGL) to access an object directly, instead of having to bind it before accessing
- Pretty much every gl function will have a version with 'named' preffix
	- ex: glBufferData > glNamedBufferData


### O que é OpenGL?
- OpenGL é uma especificação para uma API de manipulação de gráficos
- Isto é, o OpenGL não é uma API própriamente dita, mas sim uma especificação com várias implementações
- Essas implementações são feitas pelos produtores de placas gráficas, semelhante a como os compiladores de C++ são feitos pelos fabricantes de CPU
- As implementações do OpenGL são feitas em C, mas podem ser traduzidas para outras linguagens


### APIs relacionadas:
- OpenGL ES: API multiplataforma para gráficos 2D e 3D em sistemas embarcados, como celulares, consoles e veículos
    - Implementa apenas uma parte da especificação, mas tem funcionalidade adicional
- WebGL: API cross-platform para gráficos 3D na web
    - Bastante semelhante ao OpenGL ES, com algumas diferenças devido a limitações do JavaScript


### Onde eu encontro o código OpenGL para adicionar na minha aplicação?
- No Windows, a DLL do OpenGL fica na pasta system32
- Essa DLL é responsável por carregar a implementação do OpenGL da placa de video


### Como o OpenGL funciona?
- O OpenGL é como se fosse uma grande **máquina de estados**;
    - Pensar nele dessa forma vai ajudar a entendê-lo mais facilmente;
- Esse estado é armazenado em algo chamado de **Context**;
- Quando usamos o OpenGL, geralmente mudamos o seu estado através de opções, manipulação de buffers etc;
- Após fazermos as modificações necessárias no estado, solicitamos que o OpenGL utilize-o para desenhar na tela;
- No OpenGL, basicamente existem dois tipos de funções:
    - 1. As que modificam o estado;
    - 2. As que usam o estado para fazer algo;


### Onde ficam as funções extra da minha placa de video moderna?
- Essas funções são implementadas no OpenGL por meio de extensions
- Extensions podem ser carregadas através de bibliotecas como Glad que as buscam nos drivers da placa de video


### Principais conceitos:
- Context: Contém o estado atual do OpenGL
- Object: Coleção de opções que representam parte do estado do OpenGL
    - Podem ser visualizados como structs
    - Um objeto pode conter as configurações de uma janela, como tamanho, quantas cores suporta etc
    - Exemplos de objetos são buffers, texturas, vertex array, samplers etc
- Extensions: São funções incluídas pelos fabricantes de placa de video que não estão na especificação principal do OpenGL
    - Essas funções podem ser carregadas através da função wglGetProcAddress
    - Mas por ser trabalhoso, é possível usar um loader como Glad para carregá-las


### Como linkar o OpenGL na minha aplicação?
- No CMake, linkar a lib opengl32 ao target desejado
    - O CMake irá encontrar a DLL do OpenGL automaticamente
- Além disso, é preciso usar um **loader** para **carregar as funções do OpenGL** da sua placa de video
    - Examplos de loaders são **glad** e **glew**;

```cmake

```


### Uso típico de objetos:

- Substitui-se {object} pelo objeto em questão (buffers, framebuffers etc);
```
    // create object
    unsigned int objectId = 0;
    glGen{Object}(1, &objectId);
    
    // bind/assign object to context
    glBind{Object}(GL_WINDOW_TARGET, objectId);

    // set options of object currently bound to GL_WINDOW_TARGET
    glSet{Object}Option(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH,  800);
    glSet{Object}Option(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
    
    // set context target back to default
    glBind{Object}(GL_WINDOW_TARGET, 0);
```


### Workflow básico:
- 0) Função para abrir uma janela no framebuffer
    - O programa irá desenhar nessa janela
- 1) Função para alocar um contexto e associar à janela
- 2) Comandos OpenGL para definir buffers, shaders, geometrias e texturas;
- 3) Comandos para desenhar a geometria, passando dados para os shaders através de buffers;



## Setup OpenGL

### Requisitos:
- OpenGL: No Windows, é instalado automaticamente junto com a SDK
    - O arquivo opengl32.dll fica na pasta system32
- GLFW: Cria uma janela que será associada ao contexto OpenGL
- Glad: Carrega funções implementadas pelos fabricantes que não estão na especificação
    - Essas funções são necessárias para desenvolver pipelines mais modernos
    - É possível carregá-las sem Glad, mas não vale a pena pois são platform-specific

### Setup:
- 1) Linkar OpenGL como DLL OpenGL
- 2) Linkar GLFW
    - GLFW precisa de OpenGL pra rodar? 
- 3) Baixar loader do Glad através do webservice https://glad.dav1d.de/
- 4) Linkar Glad



## Conceitos OpenGL

## Swap Buffers
Para que um renderer possa desenhar na tela, as informações de cor de cada pixel devem ser armazenadas em um buffer. Quando utilizamos apenas um buffer para isso, temos problemas de flicker, causado pelo tempo de resposta entre a troca do buffer anterior para o mais recente. No OpenGL isso é resolvido com o Swap Buffer, que utiliza dois buffers para desenhar na tela, sendo eles o front e back.

- Back: As saídas de comandos de render vão para esse buffer
- Front: Cor final da imagem que é mostrada na tela

O OpenGL por sua vez passa o conteúdo do back para o front buffer, então o front buffer nunca ficará vazio e o problema de flicker é resolvido.


## Pipeline gráfico

- Em OpenGL tudo fica no espaço 3D, porém a janela é um array 2D de pixels coloridos
- O pipeline gráfico do OpenGL é dividido em duas partes:
    - 1) Transformar os objetos que estão no espaço de coordenadas 3D em pixels 2D
    - 2) Transformar essas coordenadas 2D resultantes em pixels coloridos

- Essas partes por sua vez possuem vários passos, e cada passo recebe como input o output do passo anterior
- São altamente especializados e podem ser executados em paralelo em programas na GPU
- Esses programas são chamados de **Shaders**, e a função deles é processar um passo do pipeline gráfico
- O pipeline pode ser resumido nos seguintes passos:
    - 1) Input: Recebe dados de vértices como input
    - 2) Vertex shader: Input = vértices, output = vértices modificados
    - 3) Shape assembly: Input = vértices, output = forma primitiva
    - 4) Geometry shader: Input = primitive, output = geometria completa
    - 5) Tests, blending: Input = geometria, output = fragmentos
    - 6) Fragment shader: Input = fragmentos, output = fragmentos modificados
    - 7) Rasterização


### Vertex buffers:
- Buffers são um dos tipos de objeto do OpenGL
- Nada mais são do que arrays de dados de tipo genérico que serão consumidos por shaders
- Ou seja, buffers podem armazenar vários tipos diferentes, tudo depende de qual informação queremos passar pros shaders
- Para enviarmos dados de vértices para um shader, é preciso:
    - 1) Criar um vertex buffer
    - 2) Vinculà-lo a um tipo de buffer
        - Só um buffer pode estar vinculado a um tipo por vez
        - Uma vez vinculado, todas as funções usadas no tipo especificado serão aplicadas a ele
    - 3) Passar os vértices para o buffer
    - 4) Especificar como os dados do buffer devem ser interpretados pelo shader
    - 5) No shader, declarar um atributo com a keyword in, que receberá dados do buffer
- Esse processo é todo **feito pela CPU**, então o ideal é passar sempre o maior número de vértices possível de uma só vez
    - Isso é feito para evitar um gargalo, pois a CPU é muito mais lenta que a GPU

```
unsigned int vbo;
glGenBuffers(1, &vbo);	// Criar um buffer
glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Vincula o buffer ao tipo GL_BUFFER_ARRAY

// A função glBufferData é feita especificamente pra copiar dados definidos pelo usuário para buffers
// Seu último parâmetro pode assumir 3 valores:
// GL_STREAM_DRAW: Os dados são settados apenas uma vez e usada pela GPU poucas vezes
// GL_STATIC_DRAW: Os dados são settados apenas uma vez e usados várias vezes
// GL_DYNAMIC_DRAW: Os dados são settados e usados várias vezes
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copia os vértices para o buffer

// Especifica como os atributos do VBO devem ser usados pelo vertex shader, os atributos são:
// index: indice da VBO que queremos configurar, nesse caso é 0 (valor passado para glEnableVertexAttribArray e para layout dentro do shader)
// size: número de atributos por valor (3 para vec3, 4 para vec4 etc)
// type: tipo de cada valor
// normalized: especifica se queremos que os dados sejam normalizados automaticamente ao passar
// stride: distância em bytes entre o início de um atributo e o início do próximo. nesse caso é o tamanho de 3 floats (vec3)
// pointer: offset para dizer a partir de que posição do array podemos ler o dado que estamos configurando
glEnableVertexAttribArray(0);

glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
```

#### Atributos extra
- É possível passar vários tipos de atributos diferentes via vertex buffer

```
float vertexData[] = {
    // vertices         // cores
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // direita
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // esquerda
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // cima
};

// Posição do vértice
// O stride tem tamanho 6 * sizeof(float), 3 floats do vértice + 3 floats da cor
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);       // O shader deve buscar em layout = 0

// Cor
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
glEnableVertexAttribArray(1);       // O shader deve buscar em layout = 1
```

- Observe que um atributo sempre deve estar associado a um vértice

### Vertex Array Object

- VAOs servem para guardar configurações de VBOs, evitando a necessidade de repetí-las toda vez que se deseja desenhar algo
- A sintaxe de criação de uma VAO é como a de qualquer objeto
- Para usar uma VAO, é preciso dar bind nela e só depois criar/configurar uma VBO
- Apenas uma VAO pode ser bindada por vez
- Isso significa que VBOs criadas após o bind serão associadas a essa VAO
- Além do bind antes da configuração, é preciso dar bind antes de desenhar, pois apenas a VAO bindada será desenhada

```

unsigned int vao;
glGenVertexArrays(1, &vao);
glBindVertexArray(vao);

// === Configura VBOs ===
...
// === Configura VBOs ===

glBindVertexArray(vao);
openGLFunctionsToDrawThings();
```

### Desenhando na tela
-  Após definir os buffers e arrays, basta seguir os passos:

```
glUseProgram(shaderProgram);        // 1) Determinar qual shader será utilizado
glBindVertexArrays(vao);            // 2) Bindar a VAO que será desenhada

// arg1: tipo do objeto
// arg2: indice do vertex array que queremos desenhar (os vertex attribs foram bindados em 0)
// arg3: Quantos vértices serão desenhados
glDrawArrays(GL_TRIANGLES, 0, 3);   // 3) Desenhar os vértices, nesse caso especificando que formam um triângulo
```


### Element Buffer Objects
- Um dos problemas de desenhar itens usando apenas VBOs é que os vértices precisam ser declarados em ordem para cada triângulo
- Isso significa que mesmo que dois objetos tenham vértices iguais, eles não podem ser reaproveitados
```
// Perceba que os triângulos compartilham seus vértices 2 e 3
float vertices[] = {
    // Triângulo 1
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,

    // Triângulo 2
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};
```

- Para resolver esse problema, é preciso declarar um EBO (Element Buffer Object)
    - Ele possui índices para os vértices de um objeto
    - Esses índices dizem ao OpenGL quais vértices de um VBO devem ser usados para desenhar um triângulo
    - Isso significa que precisamos apenas declarar os vértices únicos e os indices de cada triângulo

```
float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,        // Triângulo 1
    1, 2, 3         // Triângulo 2
};
```

- Eles são declarados iguai a qualquer buffer
    - Assim como as VBOs, os VEOs ficam associados ao VAO atual

```
unsigned int ebo;
glGenBuffers(1, &ebo);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

- Para desenhar elementos a partir de um EBO, utiliza-se a função `glDrawElements`
    - O EBO é passado para o VAO, então só é preciso dar bind nele se ele não tiver em um VAO 

```
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// arg1: type of object
// arg2: number of indices
// arg3: type of the indices
// arg4: index to start reading from ebo (offset)
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

### Diferentes formas de desenhar um triângulo
A partir do OpenGL 3.3 Core, o uso de VAOs se tornou obrigatório, então um triângulo pode ser desenhado usando as duas abordagens:

- VAO + VBO + glDrawArrays
- VBO + EBO + glDrawElements

- Obs: O VAO passa os buffers pra função de draw
    - Isso significa que a função de draw depende de quais buffers foram bindados


## Debugging
- Wireframe mode:

```
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      // Desenha os objetos como linhas, sem preenchimento de cor (wireframe)
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      // Desenha os objetos com cores (sem wireframe)
```