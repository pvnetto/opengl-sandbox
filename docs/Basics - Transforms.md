# Transformações

Transformações são usadas para **converter os vértices** de uma malha em dados que podem ser usados pelo pipeline gráfico para **determinar como ou se os objetos devem ser desenhados na tela**. Essas transformações são dadas por **matrizes**, então é interessante pensar nelas como **funções ou transformações lineares**.

## Espaços

Inicialmente temos apenas os vértices em espaço local, com o centro do próprio objeto como origem, e então aplicamos diversas matrizes até que tenhamos informações não só de **como o objeto está posicionado no mundo**, mas também de **como ou se ele será visualizado pela câmera**.

Pode-se dizer então que as transformações são usadas para **converter as coordenadas de um objeto entre diferentes espaços**, até que elas deixem de ser informações apenas do objeto e passem a ser uma **representação precisa de como o objeto será visualizado**.

Uma forma de pensar sobre esses espaços é que 


### Local

O espaço local é o estado inicial de um objeto, onde suas coordenadas 

### World

### View

### Clip

## Matrizes

### Model

### View

### Projection

### Juntando as matrizes: MVP