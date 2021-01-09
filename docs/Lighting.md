# Iluminação

## Tipos de luz

### Direcional

### Point

### Spotlight


## Phong
Na vida real, um fóton (partícula de luz) ricocheteia numa cena até que seja totalmente absorvido, fazendo com que **atinja vários objetos de forma indireta**. Cada vez que atinge um objeto, este absorve um pouco da luz, o que afeta a forma como nós, humanos, o visualizamos, pois a cor da luz se mistura com a do objeto, e essa mistura depende de vários fatores, como a taxa de absorção/refração do material etc. Esse efeito ricochete é o que torna **difícil um objeto ser totalmente escuro**, pois eventualmente alguns fótons acabam atingindo-o, mesmo que sejam poucos.

Reproduzir esse fenômeno de forma realista é computacionalmente caro, por isso na computação gráfica fazemos **modelos de iluminação**, que são **aproximações do modelo físico da vida real**.

O modelo mais básico de iluminação possui 3 componentes: **ambiente**, **difuso** e **especular**, e é conhecido como **modelo Phong**. Modelos mais complexos possuem mais componentes, porém são mais complexos de implementar e exigem mais poder computacional.

Em geral, esses modelos **são implementados no fragment shader**, porém também é possível fazê-los diretamente no vertex shader. Quando calculado no vertex shader, o modelo deixa de ser Phong e passa a ser chamado de Goraud.


### Ambiente
Em um modelo complexo com iluminação global, calculamos a refração da luz pela cena, então lugares que não são iluminados diretamente acabam recebendo uma porção da luz. Como esse modelo é muito pesado, podemos **simular esse efeito através da luz ambiente**.

A luz ambiente nada mais é do que uma **fonte de luz sem posição nem direção, aplicada em todos os objetos da cena**. Isso significa que com a luz ambiente, **os objetos nunca serão totalmente escuros**. A luz ambiente necessita apenas de dois parâmetros: **intensidade** e **cor**. 

```
vec3 ambient = ambientColor * ambientIntensity;
```

Existem duas formas principais de implementar luz ambiente:
- 1) Aplicar para cada fonte de luz, isto é, todas as fontes de luzes irão contribuir no valor final da luz ambiente
- 2) Usar apenas uma cor que representa o valor total da luz ambiente (na Unity é feito assim)


### Difuso
É o componente principal de um modelo de iluminação. Ele é responsável por **calcular o brilho de um fragmento**, e para isso utiliza o **ângulo entre a direção da luz e a normal do fragmento**, isto é, se um feixe de luz estiver **perpendicular a superfície (0 graus) alinhado com a normal, ele terá 100% de brilho**, e a partir do ponto que estiver **perpendicular a normal (90 ou mais graus), não terá brilho**.

Então para calcular a intensidade do componente difuso, basta tirar o **produto escalar entre entre a normal e o vetor de direção da luz**. Isso irá resultar em um valor **escalar no intervalo [-1, 1]**, onde 1 significa 100% de brilho e qualquer valor menor ou igual a 0 significa 0% de brilho.

```
float intensity = dot(normal, lightDirection);
intensity = max(intensity, 0.0f);       // Impede que o valor seja menor que 0, para não termos cores negativas
vec3 diffuse = intensity * lightColor;
```

Perceba que **o vertex shader é reponsável por passar as normais para o fragment shader**.

```
layout (location = 1) in vec3 aNormal;
out vec3 normal;

void main() {
    normal = aNormal;
}
```

Obs: Como o vetor normal é uma direção, ele não sofre influencia da translação no objeto, porém **se o objeto passar por uma escala não-uniforme ou rotação, a normal deve ser recalculada** antes de a usarmos no shader. Para isso, precisamos **multiplicar a normal** pela **matriz normal**, que pode ser obtida calculando a **transposta da inversa da parte superior esquerda 3x3 da matriz modelo**. Usamos apenas a porção 3x3 superior esquerda porque essa é a parte que tem apenas escala e rotação, ignorando a translação

```
mat3 normalMatrix = mat3(transpose(inverse(model)));
```

A **direção da luz é calculada a partir do fragmento, e não da fonte de luz**. Apesar de ser contra-intuitivo, isso **facilita na hora de calcular o ângulo** entre a normal e a direção da luz, pois elimina a necessidade de inverter o resultado do produto escalar.

```
vec3 lightDir = normalize(lightPos - fragPos);   // Calculamos a direção do fragmento para a fonte de luz, e não o contrário
```


### Especular
A especular **simula o efeito de refração da luz** em uma superfície "brilhosa".

**Como calcular a especular**:
- 1) Calcula a direção da luz
- 2) Reflete a direção da luz ao redor da normal 
- 3) Calcula a direção da view
- 4) Faz o produto escalar entre a direção da luz refletida e a direção da view
    - Semelhante ao que se faz no componente difuso
- 5) Calcula a especular fazendo a potência entre o coeficiente de brilho do objeto e o resultado do produto escalar
    - Quanto maior o coeficiente de brilho, menos ele se espalha na superfície
        - Valores altos (64+): +Brilho, -Área
        - Valores baixos (64-): -Brilho, +Área


```
vec3 lightDir = normalize(fragPos - lightPos);
vec3 lightDirReflected = reflect(-lightDir, normal);    // reflect espera uma direção vinda da fonte de luz, por isso usamos a direção negativa
vec3 viewDir = normalize(fragPos - viewPos);

float shininess = 32;
float specularIntensity = max(dot(lightDirReflected, viewDir), 0.0f);
specularIntensity = pow(specularIntensity, shininess);

vec3 specular = specularIntensity * lightColor;
```

Obs: Assim como a direção da luz, a **direção da view** também é **calculada a partir do fragmento** para facilitar o produto escalar


### Combinando os componentes

```
vec3 finalColor = (ambient + diffuse + specular) * objectColor;
```