#json parser and creator library in C

> tanto.h

#Using tanto
To use tanto you can include tanto.h header file.

```
lang=c
#include "tanto.h"
```

#typedef in tanto.h
```
TJSON_t
```

#example usage
```
lang=c
TJSON_t *json = NULL;
	
TJSON_t *node1 = tanto_init_node("hello", "world!", 1);
TJSON_t *node2 = tanto_init_node("parser", "tanto", 1);
tanto_push(&json, node1);
tanto_push(&json, node2);

tanto_write_file("hello_world.json", json);
```
