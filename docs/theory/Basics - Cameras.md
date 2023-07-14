# Cameras
No mundo da renderização 3D, utilizamos câmeras virtuais para que possamos visualizar a cena a partir de diferentes ângulos. Para descrever uma câmera virtual, precisamos de algumas propriedades básicas, como **posição**, **direção**, **vetores right/up (perpendiculares a direção)** e o tipo de **projeção**, e a partir dessas informações, podemos **montar uma matriz view que representa a câmera**.

## Projeções
Câmera podem ter projeção **ortográfica** ou **perspectiva**. Para mais detalhes, verificar a seção projection em [Basics - Transforms].

## Matriz LookAt
A matriz LookAt é um **tipo especial de matriz view** que cria um sistema de coordenadas onde o usuário está **olhando um ponto a partir de uma posição**.

A fórmula para montar a matriz LookAt de uma câmera é dada por:
```
right   = (r1, r2, r3)
up      = (u1, u2, u3)
direção = (d1, d2, d3)

base =  [r1][r2][r3][0]
        [u1][u2][u3][0]
        [d1][d2][d3][0]
        [0][0][0][1]

posição =       (v1, v2, v3)
translação =    [0][0][0][v1]
                [0][0][0][v2]
                [0][0][0][v3]
                [0][0][0][1]

LookAt = base^T * -translação
```

**Perceba que:**
- A matriz *base* representa o espaço vetorial da câmera, e é resposável pela rotação da view
- Em LookAt, utilizamos a **transposta de base** e a **negativa da translação**, pois a função da matriz view é **mover o mundo na direção oposta da câmera**  

## Calculando os vetores base da câmera

### Direção (forward)
O vetor de direção é o mais complexo entre os 3 vetores base, pois ele deve **considerar os ângulos de rotação da câmera** para ser calculado.

Se utilizarmos euler para rotacionar, precisaremos **apenas dos ângulos yaw e pitch**, já que câmeras dificilmente são rotacionadas ao redor de Z, e como as câmeras costumam ser rotacionadas **mais ao redor de Y do que X**, nossa **hierarquia de rotação deve ser Y (yaw) > X (pitch)**. Isso significa que **yaw irá influenciar os X, Y, Z** e **pitch apenas X e Z**. Temos então a seguinte fórmula:

```
glm::vec3 cameraDirection = glm::vec3(
    glm::cos(pitch) * glm::cos(yaw),
    glm::sin(yaw),
    glm::sin(pitch) * glm::cos(yaw)
);
```

### Right
O vetor right é **perpendicular à direita ao vetor de direção**. Ele pode ser obtido através do **produto vetorial entre a direção e o vetor up do mundo**.

```
glm::vec3 worldUp (0, 1, 0);
glm::vec3 cameraRight = glm::cross(cameraDirection, worldUp);
```

### Up
O vetor up é **perpendicular para cima aos vetores de direção e right**. Ele pode ser obtido através do **produto vetorial entre a direção e o vetor up**.

```
glm::vec3 up = glm::cross(cameraDirection, cameraRight);
```