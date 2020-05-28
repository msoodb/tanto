# json parser and creator library in C, under development, not ready to use!

> tanto.h

# Using tanto
To use tanto you can include tanto.h header file.

```
#include "tanto.h"
```

# typedef in tanto.h
```
TJSON_t *json;
```

# example usage
```
TJSON_t *json = NULL;
TANTO_INIT(&json);


TJSON_t *server = tanto_create_node(TANTO_JSON_OBJECT, "server", NULL);
tanto_push(&json, server);

TJSON_t *path = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "path", "http://www.gnu.org/licenses/");
tanto_push(&json, path);


TJSON_t *parser = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "parser", "tanto");
TJSON_t *library = tanto_create_node(TANTO_JSON_OBJECT, "library", NULL);
TJSON_t *lex = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "lex", "lexer");	
tanto_push(&json, parser);
tanto_push(&server, library);
tanto_push(&server, lex);

TJSON_t *usage = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"usage", "example.c");
TJSON_t *tests = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"tests", "test.c");	
tanto_push(&library, usage);	
tanto_push(&library, tests);


TJSON_t *color = tanto_create_node(TANTO_JSON_OBJECT_FIELD, "color", "red");
tanto_push(&json, color);

TJSON_t *main = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"main", "main.c");	
tanto_push(&json, main);


TJSON_t *functions = tanto_create_node(TANTO_JSON_ARRAY, "functions", NULL);
tanto_push(&json, functions);
TJSON_t *pop = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"pop", NULL);	
tanto_push(&functions, pop);
TJSON_t *push = tanto_create_node(TANTO_JSON_OBJECT_FIELD ,"push", NULL);	
tanto_push(&functions, push);

tanto_write_file("../json/example2-output.json", json);
```

## make examples

```
$ make example
````

```
$ ./example1
$ ./example2
````
