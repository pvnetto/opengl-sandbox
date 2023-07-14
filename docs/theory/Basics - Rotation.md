# Rotações

## Ângulos de euler
Ângulos de euler são valores que **descrevem a rotação de um objeto** ao redor dos 3 eixos num **plano cartesiano**.

### Pitch, Yaw e Roll
- **Pitch**: Rotação ao redor do eixo **X**
- **Yaw**: Rotação ao redor do eixo **Y**
- **Roll**: Rotação ao redor do eixo **Z**

### Visualizando as rotações
Uma boa referência para visualizar como as rotações funcionam é [nesse video](https://www.youtube.com/watch?v=zc8b2Jo7mno) sobre gimbal lock.

**Yaw:** 
- Desenhar círculo unitário ao redor de Y
- Perceber que cosseno = x e seno = z
- Dado yaw = θ, cosθ = x e senθ = y:
    - dir = vec3(cosθ, 0, senθ)

**Pitch:**
- Desenhar círculo unitário ao redor de X
- Perceber que cosseno = z e seno = y
- Dado pitch = B, cosB = z e senB = y
    - dir = vec3(0, senB, cosB)

**Roll:**
- Desenhar círculo unitário ao redor de Z
- Perceber que cosseno = x e seno = y
- Dado roll = α, cosα = x, senα = y
    - dir = vec3(cosα, senα, 0)

### Hierarquia
Apesar de não existir ordem correta para rotação, essa ordem importa, pois quando usamos ângulos de euler, **cada rotação pode influenciar nos outros eixos de rotação**. Quando um eixo é influenciado isso significa que ele é rotacionado.

Ex: Se rotacionarmos um objeto 45 graus ao redor de Y, os eixos de rotação de X e Z serão rotacionados em 45 graus, logo rotações ao redor de X ou Z não irão considerar o eixo perpendicular a origem, mas sim o eixo rotacionado.

Por esse motivo, é importante **estabelecer uma hierarquia (ordem) de rotação**, pois ângulos euler no topo da hierarquia influenciam o eixo de rotação dos de baixo, mas os de baixo não influenciam os de cima.

Ex: Dada uma hierarquia de rotação Y > X > Z:
- Y influencia X, Y, Z
- X influencia X e Z
- Z influencia Z

Ao escolhermos a hierarquia de rotação para um objeto, devemos considerar **quais ângulos de rotação são mais utilizados**, e estes devem ficar no **topo da hierarquia**.