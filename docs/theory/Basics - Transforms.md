# Transformações

Transformações são usadas para **converter os vértices** de uma malha em dados que podem ser usados pelo pipeline gráfico para **determinar como ou se os objetos devem ser desenhados na tela**. Essas transformações são dadas por **matrizes**, então é interessante pensar nelas como **funções ou transformações lineares**.

## Espaços

Inicialmente temos apenas os vértices em espaço local, com o centro do próprio objeto como origem, e então aplicamos diversas matrizes até que tenhamos informações não só de **como o objeto está posicionado no mundo**, mas também de **como ou se ele será visualizado pela câmera**.

Pode-se dizer então que as transformações são usadas para **converter as coordenadas de um objeto entre diferentes espaços**, até que elas deixem de ser informações apenas do objeto e passem a ser uma **representação precisa de como o objeto será visualizado**.

Uma forma de pensar sobre esses espaços é que cada um tem sua própria origem, então quando transformamos de um espaço para outro, é **como se estivéssemos convertendo uma coordenada de uma origem para outra**.

O principal motivo para utilizarmos espaços, no entanto, é porque algumas operações fazem mais sentido em certos espaços do que outros. Ex: Para modificar um modelo diretamente usamos o espaço local, mas se quisermos posicioná-lo, o ideal é usar o world.


### Local

O espaço local é o **estado inicial das coordenadas um objeto**, que são calculadas a partir de uma origem relativa ao próprio objeto. Quando **importamos um modelo** de um software de modelagem 3D, recebemos as coordenadas do objeto em espaço local.


### World
World é o nome dado ao espaço onde as **coordenadas do mundo do jogo ficam**, isto é, a posição do objeto nesse espaço é equivalente a **posição do objeto no mundo do jogo**. Ao importarmos um modelo e o colocarmos na cena, geralmente ele é posicionado na origem do mundo. Então precisamos **utilizar transformações para posicioná-lo em um lugar que faça sentido**.

A matriz responsável em converter coordenadas do **espaço local para world** é a **matriz model**, que é uma **combinação das matrizes de transformação**, isto é, translação, rotação e escala.

### View
Também conhecido como **espaço cãmera ou eye**, é o espaço **responsável pela câmera** do jogo, ou seja, as coordenadas desse espaço **consideram a câmera como origem**.

A matriz que converte coordenadas de **world pra view** é a **matriz view**. Essa matriz é **semelhante a model**, sendo que as **transformações são invertidas** e relativas a câmera. Ex: Se a câmera está na posição (5, 5, 3) e rotação (30, 0, 0), as coordenadas são transladadas em (-5, -5, -3) e rotacionadas em (-30, 0, 0).

Uma forma de visualizar esse espaço é **pensar em como os objetos ficariam se a origem do mundo fosse reposicionada para a posição da câmera**

### Clip
Esse é o espaço em que as coordenadas devem estar no final da execução do vertex shader. Ele **determina se um objeto será ou não desenhado** na tela baseado nas coordenadas view do objeto em relação ao **frustrum da câmera**. Outra forma de pensar sobre o clip space é que ele converte as da view para coordenadas 2D relativas a tela, **normalizadas entre [-1.0, 1.0]**.

A matriz que converte as coordenadas de **view para clip** é a **matriz projection**. Ao final da conversão, coordenadas que tiverem algum **eixo com valor fora do intervalo [-1.0, 1.0] não são desenhadas** na tela.


## Matrizes

### Model
A matriz model é formada pelas transformações aplicadas no objeto, isto é, **translação**, **rotação** e **escala**.

```
M = T * R * S
```

No OpenGL, poderíamos definir a matriz model da seguinte forma:

```
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(0.5f, 0.2f, 0.0f));
model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::scale(model, glm::vec3(1.2f, 1.2f, 1.2f));
```

### View
A matriz view é formada pelo inverso das transformações aplicadas na câmera, isto é, **o inverso da translação e rotação**. Isto porque o objetivo dessa matriz é converter coordenadas de world para view, a view tem a câmera como origem.

```
V = -T * -R
```

A matriz view é definida de forma **semelhante a model**, porém os **vetores são invertidos**:

```
glm::mat4 view = glm::mat4(1.0f);
view = glm::translate(view, -glm::vec3(1.0f, 0.0f, 1.0f));
view = glm::rotate(view, -glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
```

### Projection
A matriz de projeção é a mais complexa entre as 3. Ela é responsável por converter as coordenadas da **view para o frustrum da câmera**. Essa conversão é feita transformando coordenadas da view em coordenadas 2D relativas a tela, **normalizadas entre [-1.0, 1.0]**.

A formação da matriz de projeção depende da configuração da câmera, que pode ser **ortográfica** ou **perspectiva**.

**Ortográfica**
A projeção ortográfica **define o frustrum como um cubo**, e **todos os vértices fora dele são clippados** da tela. Essa projeção é mais simples de fazer, e é definida por 3 parâmetros: **altura**, **largura** e **planos near/far**. Altura e largura são auto-explicativos, mas os planos near/far definem respectivamente as distâncias mínima e máxima que um objeto deve estar da câmera para que não seja clippado, ou seja, objetos com **distância menor que near ou maior que far são clippados**.

>> ADICIONAR IMAGEM <<

Criar uma matriz de projeção ortográfica no OpenGL é bem simples, basta usar a função `glm::ortho` da *glm*.

```
float width = 800.0f, height = 600.0f, near = 0.1f, far = 0.1f;
glm::mat4 = glm::ortho(0.0f, width, 0.0f, height, near, far);
```

**Perspectiva**
Essa projeção é a mais **semelhante a uma câmera da vida real**, e objetos **distantes da câmera parecem menores para o observador**. Ela **define o frustrum como uma caixa não uniforme**, próximo a uma pirãmide. Diferente da ortográfica que usa dimensões absolutas, a perspectiva utiliza como parâmetros **field of view (fov)** e **aspect ratio**, além dos planos near/far. Isto porque cada seção do frustrum tem tamanho variável dependendo do quão distante está da câmera, então a altura é calculada usando o fov e a largura é calculada usando a altura e o aspect ratio.

>> ADICIONAR IMAGEM <<

Uma das particularidades desse tipo de projeção é que ela **manipula o valor de *w* de cada vértice** de forma que quanto mais longe da câmera, maior. Após a transformação, os **componentes (x, y, z) do vetor são divididos por *w***, resultando em coordenadas normalizadas em NDC (-1.0, 1.0).

Para criar uma matriz de projeção perspectiva no OpenGL, basta usar a função `glm::perspective`
```
float fov = 45.0f, aspectRatio = 1920.0f / 1080.0f, near = 0.1f, far = 100.0f;
glm::mat4 projection = glm::perspective(fov, aspectRatio, near, far);
```


### Juntando as matrizes: MVP

Após calcular as matrizes, model, view e projection, devemos **juntá-las em uma só transformação e aplicar ao vértice**, para transformar as coordenadas em **NDC no espaço clip**. Vale sempre lembrar que **a ordem que multiplicamos as matrizes é importante**. Nesse caso, fazemos:

```
Vclip = Mproj * Mview * Mmodel * V
```

No OpenGL, essas matrizes são formadas na CPU e **passadas para o shader via uniforms**.