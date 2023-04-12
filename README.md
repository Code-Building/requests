# Requests

C++ Windows-Based HTTP Library for facility and easiness

### Code Building


#### Join us:

Join CodeBuilding

https://codebuilding.org

## Examples


GET Request Example
```cpp
#include <requests/http.h> // Let's include requests default header

requests::Request* request = resquests::get("httpbin.org/get");

printf( "%s", request->status_code.c_str() );

// Output:

200 OK
```

#### For the following php script:
```php
<?php
echo $_POST['parameter1'];
echo $_POST['parameter2'];
?>
```
POST Request example
```cpp
#include <requests/http.h>

requests::post_data data = {
  {"parameter1", "value1"},
  {"parameter2", "value2"}
};

requests::Request* request = requests::post( "localhost/example.php" , data );

printf( "%s", request->text.c_str() );

// Output:

value1value2
```

test
## License

