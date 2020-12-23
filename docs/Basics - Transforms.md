# Transformações

Transformações são usadas para **converter os vértices** de uma malha em dados que podem ser usados pelo pipeline gráfico para **determinar como ou se os objetos devem ser desenhados na tela**. Essas transformações são dadas por **matrizes**, então é interessante pensar nelas como **funções ou transformações lineares**.

## Espaços

Inicialmente temos apenas os vértices em espaço local, com o centro do próprio objeto como origem, e então aplicamos diversas matrizes até que tenhamos informações não só de **como o objeto está posicionado no mundo**, mas também de **como ou se ele será visualizado pela câmera**.

Pode-se dizer então que as transformações são usadas para **converter as coordenadas de um objeto entre diferentes espaços**, até que elas deixem de ser informações apenas do objeto e passem a ser uma **representação precisa de como o objeto será visualizado**.

Uma forma de pensar sobre esses espaços é que cada um tem sua própria origem, então quando transformamos de um espaço para outro, é **como se estivéssemos convertendo uma coordenada de uma origem para outra**.   


### Local

O espaço local é o **estado inicial das coordenadas um objeto**, que são calculadas a partir de uma origem relativa ao próprio objeto. Quando **importamos um modelo** de um software de modelagem 3D, recebemos as coordenadas do objeto em espaço local.


### World



### View

### Clip

## Matrizes

### Model

### View

### Projection

### Juntando as matrizes: MVP