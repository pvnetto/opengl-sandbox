# Matrizes

Matrizes por si só são apenas **um construto matemático para armazenar com dados arbitrários**, porém elas possuem operações e propriedades que possibilitam que sejam **usadas como funções ou transformações lineares**. Por essa razão, existem matrizes especiais cujos resultados são previsíveis quando operadas com outros vetores/matrizes.

Ex: Matriz identidade, matriz de rotação, translação etc. Se pegarmos qualquer uma dessas matriz e operarmos com uma outra (ou vetor), o resultado será previsível.



## Operações

### Adição
Possui as **mesmas propriedades que adição de números reais**, porém aplicado a matrizes.

**Definição**: Dadas duas matrizes *A* e *B*, o resultado *C* é uma matriz de mesma dimensão, cujo elemento *Ci,j* = *Ai,j* + *Bi,j*. Isto é, faz-se uma soma de elemento a elemento entre as duas matrizes.

**Resumo**: Soma-se elemento a elemento; As matrizes precisam ter a mesma dimensão.

**Propriedades**:
- Closure: A + B resulta em uma matriz de mesma dimensão
- Comutativa: A + B = B + A 
- Associativa: (A + B) + C = A + (B + C)
- Identidade: A + O = A
- Inversa: A + (- A) = O

### Multiplicação por escalar
Possui as **mesmas propriedades que multiplicação de números reais**, porém todos os elementos da matriz são multiplicados pelo escalar.

**Definição**: Dado uma matriz *A* e um escalar x, a matriz C resultante de A . x é uma matriz de dimensão igual a A, e cada elemento *Ci,j* = *Ai,j* . x

**Resumo**: Todos os elementos são multiplicados um a um pelo escalar

**Propriedades**: Dados escalares x, y e matriz A
- Associativa: (xy)A = x(yA)
- Distributiva:
    - x(A + B) = Ax + Bx
    - (x + y)A = Ax + Ay
- Identidade: 1A = A
- Multiplicação por zero:
    - A . 0 = 0
    - x . 0 = 0
- Closure: xA tem as mesmas dimensões de A

### Multiplicação matriz x matriz

A multiplicação entre matrizes é um caso bem diferente das operações anteriores, pois é **bastante diferente da multiplicação entre números reais**. Sua definição é portanto contra-intuitiva e apenas uma **construção matemática que foi observada**, isso significa que as pessoas viram utilidade na multiplicação quando feita dessa forma, mas ela poderia muito bem ter sido definida de outro jeito.

**Definição**: Dadas as matrizes *A* de dimensões (i, j) e *B* de dimensões (m, n), a matriz *C* = *A* . *B* é dada pelo produto escalar entre cada elemento das matrizes *A* e *B*. Se considerarmos as linhas de *A* como vetores de linha *A0* até *Am* e as colunas de *B* como vetores de coluna de *B0* até *B*, um elemento *Cxy* é o resultado do produto escalar entre os vetores *Ax* e *By*. Isto é, fazemos em ordem o produto escalar entre cada linha de A e cada coluna de B. A matriz C deverá ter dimensões (n, i). A multiplicação de matrizes só é possível se j = m, isto é, se o número de colunas de A for igual ao número de linhas de B.

**Resumo**: Só é possível se o número de colunas de A for igual ao número de linhas de B, o resultado é o produto escalar entre linhas da matriz A e colunas da matriz B, o loop "interno" é feito em B, e o externo em A.

**Propriedades**: Dadas as matrizes A(i, j), B(m, n)
- Não-comutativa: A*B != B*A 
- Associativa: A(BC) = (AB)C
- Distributiva: A(B + C) = AB + AC
- Identidade: 1A = A
- Multiplicação por zero:
    - A . 0 = 0
    - x . 0 = 0
- Closure: A matriz resultante C tem dimensões (i, n)
    - Número de linhas de A, colunas de B

### Multiplicação matriz x vetor

Trata-se apenas de um caso especial de matriz x matriz, pois um **vetor é uma matriz com só uma linha ou só uma coluna**.



## Matrizes especiais

### Identidade
Denotada por I, a matriz identidade possui propriedades especiais **semelhantes ao número 1 no domínio dos reais**, isto é:
- A * I = A
- I * A = A

### Zero
Denotada por O, a matriz zero possui propriedades especiais, bastante **semelhantes ao número 0 no domínio dos reais**, isto é:
- A + O = A
- A - A = O
- A . O = O

### Transposta
Dada uma matriz A de dimensões (n, n), sua transposta A_t é uma matriz cujo elemento A_t(i, j) = A(j, i)



## Transformações

### Transformações lineares

Quando **multiplicamos** uma matriz A por outra matriz B, na verdade estamos fazendo uma **transformação linear** em B usando A, que é uma função que **mapeia um espaço vetorial a outro**, e nesse caso, estamos mapeando o espaço de B para o de A.

Cada coluna de uma matriz é chamada de **vetor base**, e cada um desses vetores determina a forma como a matriz mapeia cada dimensão.

A forma mais fácil de entender essa relação é através da matriz identidade: Se considerarmos uma matriz identidade I de dimensão 3x3, podemos observar que **cada coluna corresponde a uma dimensão**, isto é, a coluna 1 (1, 0, 0) corresponde a x, coluna 2 (0, 1, 0) corresponde a y etc. Isso significa que a matriz identidade mapeia de forma exata o valor de cada dimensão, sem aplicar nenhuma transformação.

```
[1][0][0]
[0][1][0]
[0][0][1]
```

De forma análoga, podemos pegar uma matriz X semelhante a matriz I, porém com as colunas invertidas:

```
[0][0][1]
[1][0][0]
[0][1][0]
```

Se multiplicarmos por uma matriz B, ela teria suas linhas invertidas. Ex:

```
[0][0][1]        [7]        [9]
[1][0][0]   *    [8]   =    [7]
[0][1][0]        [9]        [8]
```

Podemos ir um pouco além e não só inverter a ordem das colunas, mas pegar uma matriz com mais de um valor por coluna:

```
[2][3][2]
[0][2][0]
[3][0][2]
```

Se multiplicarmos por um vetor qualquer, o resultado de primeira pode não parecer intuitivo:

```
[2][3][2]        [7]        [56]
[0][2][0]   *    [8]   =    [16]
[3][0][2]        [9]        [39]
```

No entanto, podemos usar uma forma alternativa para calcular essas transformações, que deixa mais explícito como as dimensões são afetadas por cada coluna:

```
[a][b][c]   [x]       [a]       [b]       [c]
[d][e][f] * [y] = x * [d] + y * [e] + z * [f]
[g][h][i]   [z]       [g]       [h]       [i]
```

Aplicando no exemplo anterior, chegamos ao mesmo resultado, porém fica claro como cada dimensão de B é afetada pelas colunas de A
```
    [2]       [3]       [2]     [56]
7 * [0] + 8 * [2] + 9 * [0] =   [16]
    [3]       [0]       [2]     [39]
```


### Matrizes de transformação

Transformar vértices de um espaço para outro é uma das aplicações de transformações lineares. Para esse fim, existem **matrizes específicas para cada tipo de transformação**, isto é, se quisermos mover, escalar ou rotacionar um objeto, usamos matrizes específicas para cada um desses casos.

As matrizes de translação e escala são bastante intuitivas, já a de **rotação é um pouco mais complexa e diferente para cada eixo**.  

**Translação**:
A matriz de translação **soma o vetor de translação ao vértice**. Vale observar que precisamos **adicionar uma dimensão em ambas as matrizes** para que a operação funcione, porém, na API gráfica, apenas as dimensões corretas são utilizadas.

Ex: Dado um vetor de translação (t1, t2, t3):
```
[1][0][0][t1]
[0][1][0][t2]
[0][0][1][t3]
[0][0][0][1]
```

```
[1][0][0][t1]   [x]   [x + t1]
[0][1][0][t2] * [y] = [y + t2]
[0][0][1][t3]   [z]   [z + t3]
[0][0][0][1]    [1]   [1]
```

**Escala**:
A matriz de escala é **parecida com a identidade**, porém os vetores base são multiplicados pelo valor que queremos escalar em cada eixo. Isso significa que uma matriz de escala irá **multiplicar cada eixo do vértice**.

Ex: Dado um vetor de escala (s1, s2, s3)

```
[s1][0][0][0]
[0][s2][0][0]
[0][0][s3][0]
[0][0][0][1]
```

```
[s1][0][0][0]   [x]   [x * s1]
[0][s2][0][0] * [y] = [y * s2]
[0][0][s3][0]   [z]   [z * s3]
[0][0][0][1]    [1]   [1]
```

**Rotação**:
Rotação é a matriz menos intuitiva e só pode ser compreendida por completo depois de estudar álgebra linear. No entanto, **não precisamos entender a fundo como funciona** para aplicarmos. Só é importante saber que **as matrizes de rotação são diferentes para cada eixo**

Podemos encontrar essas matrizes [nesse guia](https://learnopengl.com/Getting-started/Transformations).

### Combinando as matrizes

Um dos motivos pelo qual usamos matrizes para fazer essas operações ao invés de apenas aplicá-las individualmente é que **podemos combinar as matrizes**, possibilitando **fazer todas as transformações de uma vez só**. Porém é preciso prestar atenção pois a **ordem que multiplicamos é importante**, não só porque matrizes não são comutativas, mas também porque não queremos transformar as transformações, e sim o vértice.

Ex: Se fizermos `escala * translação` a matriz resultante é uma matriz de translação multiplicada pela escala. Se aplicássemos essa matriz em um vértice, a escala ficaria correto, porém ele seria **transladado por um vetor de translação escalado**, resultando em um **comportamento indesejado**. 

```
[s1][0][0][0]   [1][0][0][t1]   [2][0][0][s1 * t1]
[0][s2][0][0] * [0][1][0][t2] = [0][2][0][s2 * t2]
[0][0][s3][0]   [0][0][1][t3]   [0][0][2][s3 * t3]
[0][0][0][1]   [0][0][0][1]     [0][0][0][1]
```

Ex: Já `translação * escala` resulta em combinação dos dois. Se aplicarmos a matriz resultante em um vértice, ele será escalado e transladado corretamente.

```
[1][0][0][t1]   [s1][0][0][0]   [s1][0][0][t1]
[0][1][0][t2] * [0][s2][0][0] = [0][s2][0][t2]
[0][0][1][t3]   [0][0][s3][0]   [0][0][s3][t3]
[0][0][0][1]    [0][0][0][1]    [0][0][0][1]
```