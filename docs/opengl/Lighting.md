# Iluminação

Na iluminação baseada em física, um fóton (partícula de luz) ricocheteia nos objetos até que seja totalmente absorvido, fazendo com que **atinja vários objetos de forma indireta**. Cada vez que atinge um objeto, este absorve um pouco da luz, o que afeta a forma como nós, humanos, o visualizamos, pois a cor da luz se mistura com a do objeto, e essa mistura depende de vários fatores, como a taxa de absorção/refração do material, quais partículas o compõem etc. Esse efeito ricochete é o que torna **difícil um objeto ser totalmente escuro**, pois eventualmente alguns fótons acabam atingindo-o, mesmo que sejam poucos.

Reproduzir esse fenômeno de forma realista é computacionalmente caro, por isso na computação gráfica fazemos **modelos de iluminação**, que são **aproximações do modelo físico da vida real**.

## Tipos de luz
No mundo real, fontes de luz podem se comportar de forma diferente dependendo do objeto que está emitindo a luz. Apesar da física dos fótons ser igual para toda luz, alguns objetos possuem propriedades especiais que fazem com que eles emitam luz de forma diferente. Ex: O sol é um objeto enorme, muito distante da terra e bastante luminoso, então os raios de luz vindos dele sempre incidem na mesma direção. Já a luz do seu quarto é um objeto pequeno e com alcance limitado, logo os fótons não viajam tanto e a direção dos fótons dependem do objeto iluminado.

Na computação gráfica, essas nuances do mundo real são modeladas por fontes de luz que se comportam de forma diferente, para que a simulação seja mais próxima ao modelo físico realista. Então apesar de todas as fontes de luz contribuírem da mesma forma para a cor final, suas propriedades são simuladas para que os raios de luz se comportem de forma parecida com o mundo real.  

Temos então 3 tipos principais de fontes de luz:

### Direcional
Todos os raios de luz vindos de uma luz direcional **incidem na mesma direção, independente da posição da fonte de luz ou do observador**, isto é, eles são praticamente paralelos. Esse modelo de fonte de luz é **semelhante ao sol**, isto é, uma **fonte de luz global que ilumina toda a cena**.


### Pontual
A luz pontual é uma fonte de luz que **ilumina em todas as direções a partir de uma posição**. Modela objetos comuns como **lâmpadas, torchas etc**. Diferente da luz direcional que não tem posição, a luz pontual tem algumas complexidades extra, como **atenuação** e **direção dependente do observador**. 

**Atenuação**:
**Reduz a intensidade da luz dependendo da distância entre a fonte e o objeto iluminado**. A atenuação é dada pela fórmula

```
attenuation = 1 / kC + kL * d + Kq * d^2

d = distância entre objeto e fonte de luz
kC = fator constante de atenuação (geralmente tem valor 1)
kL = fator da distância linear
Kd = fator da distância quadrática
```

Isso significa que valores mais altos de kL e Kd farão com que a intensidade da luz diminua mais rápido dependendo da distância. Como kL multiplica o fator linear, em geral a **intensidade será linear para distâncias pequenas até que o fator quadrático supere o linear**, e então a intensidade decairá bem mais rápido.


### Spotlight
Assim como a pontual, **ilumina a partir de uma posição e possui atenuação**. No entanto, **emite luz apenas na direção/raio especificados**, e não em todas as direções. O raio da spotlight geralmente é dado por um ângulo, então **pode ser vista como um cone**. Esse tipo de luz modela objetos como **lanternas, postes de luz, holofotes etc**.

Geralmente objetos que não estão dentro do raio da spotlight não recebem nenhuma luz, o que faz com que as **suas bordas sejam naturalmente mais grosseiras**. Para diminuir esse problema, geralmente **define-se um outro ângulo**, permitindo a definição de um raio exterior, onde **fragmentos tem uma sombra suave**, fazendo um efeito de penumbra.


## Phong
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