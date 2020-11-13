# OpenGL

### O que é OpenGL?
- OpenGL é uma especificação para uma API de manipulação de gráficos
- Isto é, o OpenGL não é uma API própriamente dita, mas sim uma especificação com várias implementações
- Essas implementações são feitas pelos produtores de placas gráficas, semelhante a como os compiladores de C++ são feitos pelos fabricantes de CPU
- As implementações do OpenGL são feitas em C, mas podem ser traduzidas para outras linguagens


### Onde eu encontro o código OpenGL para adicionar na minha aplicação?
- No Windows, a DLL do OpenGL fica na pasta system32
- Essa DLL é responsável por carregar a implementação do OpenGL da placa de video


### Como linkar o OpenGL na minha aplicação?
- No CMake, linkar a lib opengl32 ao target desejado
- O CMake irá encontrar a DLL do OpenGL automaticamente


### Como o OpenGL funciona?
- O OpenGL é como se fosse uma grande MÁQUINA DE ESTADOS
    - Pensar nele dessa forma vai ajudar a entendê-lo mais facilmente
- Esse estado é armazenado em algo chamado de Context
- Quando usamos o OpenGL, geralmente mudamos o seu estado através de opções, manipulação de buffers etc
- Após fazermos as modificações necessárias no estado, solicitamos que o OpenGL utilize-o para desenhar na tela
- No OpenGL, basicamente existem dois tipos de funções: as que modificam o estado e as que usam o estado


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


### Uso típico de objetos (substituindo a palavra object pelo objeto em questão):

```
    // create object
    unsigned int objectId = 0;
    glGenObject(1, &objectId);
    // bind/assign object to context
    glBindObject(GL_WINDOW_TARGET, objectId);
    // set options of object currently bound to GL_WINDOW_TARGET
    glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_WIDTH,  800);
    glSetObjectOption(GL_WINDOW_TARGET, GL_OPTION_WINDOW_HEIGHT, 600);
    // set context target back to default
    glBindObject(GL_WINDOW_TARGET, 0);
```


### Workflow básico:
- 0) Função para abrir uma janela no framebuffer
    - O programa irá desenhar nessa janela
- 1) Função para alocar um contexto e associar à janela
- 2) Comandos OpenGL para definir shaders, geometrias e texturas
- 3) Comandos para desenhar a geometria, passando dados dela para os shaders


### APIs relacionadas:
- OpenGL ES: API multiplataforma para gráficos 2D e 3D em sistemas embarcados, como celulares, consoles e veículos
    - Implementa apenas uma parte da especificação, mas tem funcionalidade adicional
- WebGL: API cross-platform para gráficos 3D na web
    - Bastante semelhante ao OpenGL ES, com algumas diferenças devido a limitações do JavaScript



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


