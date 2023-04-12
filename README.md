# Requests

Uma biblioteca C++ para enviar requisições em http, para facilitar na hora de programar.

### Code Building


#### Entendendo:

A biblioteca requests é baseada em **win-http**, a API do Windows para HTTP, o objetivo é enviar requisições get e post.

#### Junte-se a nós:

Junte-se a code-building um lugar para conversar sobre tecnologia, envolvendo game-hacking, Hacking e diversas outras coisas.

https://codebuilding.org

## Exemplos


Exemplo requisição GET
```cpp
#include <requests/http.h> // Vamos incluir o header padrão do Requests.

Requests::Request* resposta_requisição = Requests::get("httpbin.org/get");

printf("%s",resposta_requisição->status_code.c_str());

// Saída:

200 OK
```

#### Para o seguinte script php:
```php
<?php
echo $_POST['parametro1'];
echo $_POST['parametro2'];
?>
```
Exemplo requisição POST
```cpp
#include <requests/http.h>

Requests::post_data data = {
  {"parametro1", "valor1"},
  {"parametro2", "valor2"}
};

Requests::Request* resposta_requisição = Requests::post("localhost/example.php" , data);

printf("%s",resposta_requisição->text.c_str());

// Saída:

valor1valor2
```
## License

Licenciada abaixo da Licença [MIT](LICENSE).



