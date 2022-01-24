# Objects

**Uso típico de objetos**:

- Substitui-se {object} pelo objeto em questão (buffers, framebuffers etc);
```cpp
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